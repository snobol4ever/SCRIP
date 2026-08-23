#ifdef __cplusplus
#include "emit.h"
#include "zeta_choices.h"
#include "ir_index.h"
#include "templates/x86_asm.h"
#include "templates/bb_templates.h"
#endif
#ifdef __cplusplus
extern "C" {
#endif
#include "driver/scrip_sm.h"
#include <string>
#include "stage2.h"
#include "templates/bb_templates.h"
#include "templates/xa_templates.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>
const char * bb_src_of(const IR_t * nd);
bb_emit_mode_t  bb_emit_mode = EMIT_BINARY_WIRED;
int             g_sm_native_unsupported = 0;
FILE           *bb_emit_out  = NULL;
bb_platform_t   g_platform      = BB_PLATFORM_X86;
bb_medium_t     g_medium        = BB_MEDIUM_BINARY;
void          (*g_emit_chain_posthook)(void) = NULL;
int             g_use_sm_macros = 0;
int             g_use_bb_macros = 0;
sm_emit_t g_emit;
IR_graph_t * g_emit_cfg = (IR_graph_t *)0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
FILE *emit_outf(void) { return bb_emit_out ? bb_emit_out : stdout; }
#include <string.h>
bb_buf_t   bb_emit_buf   = NULL;
int        bb_emit_pos   = 0;
int        bb_emit_size  = 0;
bb_patch_t bb_patch_list[BB_PATCH_MAX];
int        bb_patch_count = 0;
static bb_label_t ** g_label_pool      = NULL;
static int           g_label_pool_n    = 0;
static int           g_label_pool_max  = 0;
static bb_label_t * g_flt_lbl[4] = {0, 0, 0, 0};
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void emit_label_pool_reset(void)
{
    for (int i = 0; i < g_label_pool_n; i++) free(g_label_pool[i]);
    g_label_pool_n = 0;
    g_flt_lbl[1] = g_flt_lbl[2] = g_flt_lbl[3] = (bb_label_t *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int emit_label_lookup_offset(const char * name)
{
    if (!name) return -1;
    for (int i = 0; i < g_label_pool_n; i++) if (g_label_pool[i] && g_label_pool[i]->offset != BB_LABEL_UNRESOLVED && strcmp(g_label_pool[i]->name, name) == 0) return g_label_pool[i]->offset;
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * flat_label_kind(IR_e op) { const char * n = bb_op_name(op); static char b[48]; int j = 0; if (n && n[0] == 'I' && n[1] == 'R' && n[2] == '_') n += 3; if (!n) { snprintf(b, sizeof b, "op%d", (int)op); return b; } for (; n[j] && j < 47; j++) b[j] = (n[j] >= 'A' && n[j] <= 'Z') ? (char)(n[j] - 'A' + 'a') : n[j]; b[j] = 0; return b; }
extern "C" const char * bb_kind_name(int op) { return flat_label_kind((IR_e)op); }
static int emit_floater_kind(const IR_t * n) { if (!n) return 0; if (n->op == IR_DEFINE && IR_LIT(n).ival == 1) return 1; if (n->op == IR_DEFINE && IR_LIT(n).ival == 2) return 2; if (n->op == IR_LIT_STRING && n->γ.node && n->γ.node->op == IR_CALL && IR_LIT(n->γ.node).sval && !strcmp(IR_LIT(n->γ.node).sval, "SNO$NRET")) return 3; return 0; }
static int emit_floater_member(const IR_t * n) { if (!n) return 0; if (emit_floater_kind(n)) return 1; if (n->op == IR_CALL && IR_LIT(n).sval && !strcmp(IR_LIT(n).sval, "SNO$NRET")) return 1; return 0; }
static bb_label_t * emit_floater_label(int k) { static const char * fn[4] = {0, "RETURN", "FRETURN", "NRETURN"}; if (k < 1 || k > 3) return (bb_label_t *)0; if (!g_flt_lbl[k]) g_flt_lbl[k] = emit_label_alloc("%s", fn[k]); return g_flt_lbl[k]; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
bb_label_t *emit_label_alloc(const char *fmt, ...)
{
    if (g_label_pool_n >= g_label_pool_max) {
        int new_max = g_label_pool_max ? g_label_pool_max * 2 : 64;
        bb_label_t **g = (bb_label_t **)realloc(g_label_pool, (size_t)new_max * sizeof(bb_label_t *));
        if (!g) return NULL;
        g_label_pool     = g;
        g_label_pool_max = new_max;
    }
    bb_label_t *lbl = (bb_label_t *)calloc(1, sizeof(bb_label_t));
    if (!lbl) return NULL;
    va_list ap; va_start(ap, fmt);
    vsnprintf(lbl->name, BB_LABEL_NAME_MAX, fmt, ap);
    va_end(ap);
    lbl->offset = BB_LABEL_UNRESOLVED;
    g_label_pool[g_label_pool_n++] = lbl;
    return lbl;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
bb_label_t *emit_label_intern(const char *name)
{
    if (!name) return NULL;
    for (int i = 0; i < g_label_pool_n; i++)
        if (g_label_pool[i] && strcmp(g_label_pool[i]->name, name) == 0) return g_label_pool[i];
    return emit_label_alloc("%s", name);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void bb_emit_begin(bb_buf_t buf, int size)
{
    bb_emit_buf    = buf;
    bb_emit_pos    = 0;
    bb_emit_size   = size;
    bb_patch_count = 0;
    emit_label_pool_reset();
}
extern int bb_emit_overflow;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int bb_emit_end(void)
{
    if (bb_emit_overflow) {
        bb_patch_count = 0;
        return -1;
    }
    if (bb_patch_count > 0) {
        fprintf(stderr, "bb_emit_end: %d unresolved forward reference(s):\n",
                bb_patch_count);
        for (int i = 0; i < bb_patch_count; i++)
            fprintf(stderr, "  site=%d label='%s'\n",
                    bb_patch_list[i].site,
                    bb_patch_list[i].label->name);
        abort();
    }
    return bb_emit_pos;
}
static bb_label_t *g_lblfold_from[4096]; static bb_label_t *g_lblfold_to[4096]; static int g_lblfold_n = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int seq_fold_on(void) { static int on = -1; if (on < 0) { const char *e = getenv("SCRIP_SEQ_FOLD"); on = (e && *e == '0') ? 0 : 1; } return on; }
extern "C" void bb_label_alias_reset(void) { g_lblfold_n = 0; }
extern "C" void bb_label_alias(bb_label_t *stub, bb_label_t *tgt) { if (!seq_fold_on() || !stub || !tgt || stub == tgt || g_lblfold_n >= 4096) return; g_lblfold_from[g_lblfold_n] = stub; g_lblfold_to[g_lblfold_n] = tgt; g_lblfold_n++; }
extern "C" bb_label_t *bb_label_fold(bb_label_t *l) { if (!seq_fold_on() || !l) return l; for (int hop = 0; hop < 16; hop++) { int hit = 0; for (int i = 0; i < g_lblfold_n; i++) if (g_lblfold_from[i] == l) { l = g_lblfold_to[i]; hit = 1; break; } if (!hit) break; } return l; }
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
    if (bb_patch_count >= BB_PATCH_MAX) {
        bb_emit_overflow = 1;
        return;
    }
    bb_patch_list[bb_patch_count].site  = bb_emit_pos;
    bb_patch_list[bb_patch_count].label = lbl;
    bb_patch_list[bb_patch_count].kind  = PATCH_REL32;
    bb_patch_count++;
    bb_emit_u32(0x00000000);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void bb_emit_patch_abs64(bb_label_t *lbl)
{
    lbl = bb_label_fold(lbl);
    if (!MEDIUM_BINARY) { fprintf(stderr, "bb_emit_patch_abs64: TEXT-mode reach (target='%s')\n", lbl->name); abort(); }
    if (bb_label_defined(lbl)) { bb_emit_u64((uint64_t)(uintptr_t)(bb_emit_buf + lbl->offset)); return; }
    if (bb_patch_count >= BB_PATCH_MAX) { bb_emit_overflow = 1; return; }
    bb_patch_list[bb_patch_count].site  = bb_emit_pos;
    bb_patch_list[bb_patch_count].label = lbl;
    bb_patch_list[bb_patch_count].kind  = PATCH_ABS64;
    bb_patch_count++;
    bb_emit_u64(0);
}
int bb_emit_overflow = 0;
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
        bb_emit_overflow = 1;
        return;
    }
    bb_emit_buf[bb_emit_pos++] = b;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void  bb_emit_u32(uint32_t v)  { bb_emit_byte((uint8_t)(v)); bb_emit_byte((uint8_t)(v>>8)); bb_emit_byte((uint8_t)(v>>16)); bb_emit_byte((uint8_t)(v>>24)); }
void  bb_emit_u64(uint64_t v)  { bb_emit_u32((uint32_t)(v)); bb_emit_u32((uint32_t)(v >> 32)); }
void  bb_emit_i32(int32_t v)   { uint32_t u; memcpy(&u, &v, 4); bb_emit_u32(u); }
int  g_is_text        = 0;
int  g_emit_text_mode = TEXT_MODE_INVOCATION;
int  g_emit_pos       = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void emitter_init_binary(bb_buf_t buf, int size)
{
    g_is_text = 0; g_emit_text_mode = TEXT_MODE_INVOCATION; g_emit_pos = 0;
    bb_emit_overflow = 0;
    bb_label_alias_reset();
    bb_emit_mode = EMIT_BINARY_WIRED;
    g_platform = BB_PLATFORM_X86; g_medium = BB_MEDIUM_BINARY;
    bb_emit_begin(buf, size);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void emitter_init_text(FILE *out, int mode)
{
    g_is_text = 1; g_emit_text_mode = mode; g_emit_pos = 0;
    bb_label_alias_reset();
    bb_emit_mode = EMIT_TEXT;
    g_platform = BB_PLATFORM_X86; g_medium = BB_MEDIUM_TEXT;
    bb_emit_out  = out ? out : stdout;
    emit_set_sink(out ? out : stdout);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int  emitter_end(void)        { return g_is_text ? g_emit_pos : bb_emit_end(); }
static void  ef_b1 (uint8_t a)                                   { bb_emit_byte(a); }
static void  ef_b2 (uint8_t a, uint8_t b)                        { bb_emit_byte(a); bb_emit_byte(b); }
static void  ef_b3 (uint8_t a, uint8_t b, uint8_t c)             { bb_emit_byte(a); bb_emit_byte(b); bb_emit_byte(c); }
static void  ef_b4 (uint8_t a, uint8_t b, uint8_t c, uint8_t d)  { bb_emit_byte(a); bb_emit_byte(b); bb_emit_byte(c); bb_emit_byte(d); }
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
    static const char    *mn[]    = {"jmp","je","jne","jl","jge","jg"};
    static const uint8_t  ops[6][2] = {{0xE9,0x00},{0x0F,0x84},{0x0F,0x85},{0x0F,0x8C},{0x0F,0x8D},{0x0F,0x8F}};
    int k = (int)kind < 6 ? (int)kind : 0;
    if (g_is_text) { ef_text_jmp(mn[k], target->name); g_emit_pos += 6; }
    else { if (k==0) ef_b1(0xE9); else ef_b2(ops[k][0], ops[k][1]); bb_emit_patch_rel32(target); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void emit_aligned_call_rt(const char *sym, void *addr)
{
    if (g_is_text) { emit_textf(" push rbx\n mov rbx, rsp\n and rsp, -16\n call %s@PLT\n mov rsp, rbx\n pop rbx\n", sym ? sym : ""); }
    else { ef_b1(0x53); ef_b3(0x48,0x89,0xE3); ef_b4(0x48,0x83,0xE4,0xF0); ef_b2(0x48,0xB8); bb_emit_u64((uint64_t)(uintptr_t)addr); ef_b2(0xFF,0xD0); ef_b3(0x48,0x89,0xDC); ef_b1(0x5B); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void bb_label_define(bb_label_t *lbl)
{
    if (!MEDIUM_BINARY) {
        emit_textf("%s:\n", lbl->name);
        return;
    }
    if (bb_emit_overflow) return;
    lbl->offset = bb_emit_pos;
    { static int bm = -1; if (bm < 0) { const char *e = getenv("SCRIP_BLOB_MAP"); bm = (e && *e == '1') ? 1 : 0; } if (bm) fprintf(stderr, "BLOBMAP %p %s\n", (void *)(bb_emit_buf + bb_emit_pos), lbl->name); }
    for (int i = 0; i < bb_patch_count; i++) {
        bb_patch_t *p = &bb_patch_list[i];
        if (p->label != lbl) continue;
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
            bb_emit_buf[p->site + 0] = (uint8_t)(u      );
            bb_emit_buf[p->site + 1] = (uint8_t)(u >>  8);
            bb_emit_buf[p->site + 2] = (uint8_t)(u >> 16);
            bb_emit_buf[p->site + 3] = (uint8_t)(u >> 24);
        }
        bb_patch_list[i] = bb_patch_list[--bb_patch_count];
        i--;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void emit_label_initf(bb_label_t *lbl, const char *fmt, ...)
{
    va_list ap; va_start(ap, fmt);
    vsnprintf(lbl->name, BB_LABEL_NAME_MAX, fmt, ap);
    va_end(ap);
    lbl->offset = BB_LABEL_UNRESOLVED;
}
#include "IR.h"
#include "emit_ir.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_emit_limit_init(int limit_slot_off) { fprintf(stderr, "GROUND ZERO: %s not implemented (Icon-only reset)\n", "bb_emit_limit_init"); abort(); }
#define WASM_STRTAB_MAX 4096
#define WASM_STR_DATA_BASE 0x100000
typedef struct { const char * s; int addr; int len; } WasmStrEntry;
static WasmStrEntry g_wasm_strtab[WASM_STRTAB_MAX];
static int g_wasm_strtab_n = 0;
static int g_wasm_str_next = WASM_STR_DATA_BASE;
#ifdef __cplusplus
extern "C++" {
#endif
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string wasm_emit_data_segments_str(void) {
    std::string r;
    for (int i = 0; i < g_wasm_strtab_n; i++) {
        const char * s   = g_wasm_strtab[i].s;
        int          len = g_wasm_strtab[i].len;
        int          adr = g_wasm_strtab[i].addr;
        r += emit_fmt("  (data (i32.const 0x%x) \"", adr);
        for (int j = 0; j < len; j++) {
            unsigned char c = (unsigned char)s[j];
            if (c == '"' || c == '\\') r += emit_fmt("\\%02x", (unsigned)c);
            else if (c < 32 || c > 126) r += emit_fmt("\\%02x", (unsigned)c);
            else r += emit_fmt("%c", (int)c);
        }
        r += emit_fmt("\\00\")  ;; len=%d\n", len);
    }
    return r;
}
#ifdef __cplusplus
}
#endif
#define WASM_USERFNS_MAX 256
#define WASM_MAX_PARAMS  16
typedef struct { char name[128]; int entry_pc; int nparams; char params[WASM_MAX_PARAMS][128]; } WasmUserFn;
static WasmUserFn g_wasm_userfns[WASM_USERFNS_MAX];
static int        g_wasm_userfns_n = 0;
int g_m4_dense_nid = 0;
static IR_t * g_nid_key[262144];
static int    g_nid_val[262144];
static int    g_nid_count = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int bb_node_id(IR_t * nd) {
    if (!nd) return 0;
    if (!g_m4_dense_nid) return (int)((uintptr_t)nd % 100000u);
    uintptr_t h = ((uintptr_t)nd >> 4) & 262143u;
    while (g_nid_key[h]) { if (g_nid_key[h] == nd) return g_nid_val[h]; h = (h + 1u) & 262143u; }
    g_nid_key[h] = nd; g_nid_val[h] = ++g_nid_count; return g_nid_val[h];
}
static IR_t * g_chain_entry_key[65536];
static int    g_chain_entry_n = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void emit_chain_mark_entry_emitted(IR_t * entry) {
    if (!entry) return;
    for (int i = 0; i < g_chain_entry_n; i++) if (g_chain_entry_key[i] == entry) return;
    if (g_chain_entry_n < 65536) g_chain_entry_key[g_chain_entry_n++] = entry;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int emit_chain_entry_already_emitted(const IR_t * entry) {
    if (!entry) return 0;
    for (int i = 0; i < g_chain_entry_n; i++) if (g_chain_entry_key[i] == entry) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void xa_dispatch(XA_op_t op)
{
    switch (op) {
    case XA_MACRO_LIBRARY_OPEN:    xa_macro_library_open();    return;
    case XA_MACRO_LIBRARY_CLOSE:   xa_macro_library_close();   return;
    case XA_BB_MACRO_LIBRARY:      xa_bb_macro_library();      return;
    case XA_EXEC_STMT_BLOB:        xa_exec_stmt_blob();        return;
    case XA_FILE_HEADER:           xa_file_header();           return;
    case XA_FILE_FOOTER:           xa_file_footer();           return;
    case XA_BB_PTR_SLOT:           xa_bb_ptr_slot();           return;
    case XA_ENTRY_DISPATCH:        xa_entry_dispatch();        return;
    case XA_FLAT_DATA_SECTION:     xa_flat_data_section();     return;
    case XA_PROLOGUE:              xa_prologue();              return;
    case XA_EPILOGUE:              xa_epilogue();              return;
    case XA_WASM_MAIN_OPEN:        xa_wasm_main_open();        return;
    case XA_WASM_MAIN_CLOSE:       xa_wasm_main_close();       return;
    case XA_JS_LABEL_REGISTER:     xa_js_label_register();     return;
    case XA_EXPRESSION_REGISTRY:   xa_expression_registry();   return;
    case XA_KIDS_RODATA:                                    return;
    case XA_SUB_BUILDER:                                    return;
    case XA_BUILDER:                                        return;
    case XA_REGISTRY_TABLE:                                 return;
    case XA_STRTAB_RODATA:         xa_strtab_rodata();         return;
    case XA_CSETTAB_RODATA:        xa_csettab_rodata();        return;
    case XA_CAP_FIXUP:             xa_cap_fixup();             return;
    case XA_PATTERN_BLOBS:         xa_pattern_blobs();         return;
    default: return;
    }
}
#define SMX_STRTAB_CAP 8192
static struct { const char *s; int idx; } g_strtab[SMX_STRTAB_CAP];
static int g_strtab_n = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void strtab_reset(void) { for (int i = 0; i < g_strtab_n; i++) free((void *)g_strtab[i].s); g_strtab_n = 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int strtab_intern(const char *s)
{
    if (!s) s = "";
    for (int i = 0; i < g_strtab_n; i++)
        if (g_strtab[i].s == s || strcmp(g_strtab[i].s, s) == 0) return g_strtab[i].idx;
    if (g_strtab_n >= SMX_STRTAB_CAP) { fprintf(stderr, "strtab overflow\n"); abort(); }
    int idx = g_strtab_n;
    g_strtab[g_strtab_n].s = strdup(s); g_strtab[g_strtab_n].idx = idx; g_strtab_n++;
    return idx;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void strtab_label(char *buf, size_t bufsz, const char *s)
{
    if (!s) s = "";
    int idx = strtab_intern(s);
    snprintf(buf, bufsz, ".S%d", idx);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void xa_emit_strtab_rodata(void)
{
    if (g_strtab_n <= 0) { strtab_reset(); return; }
    static const char *labels[SMX_STRTAB_CAP];
    static std::string escbuf[SMX_STRTAB_CAP];
    static const char *escaped[SMX_STRTAB_CAP];
    static char        lblbuf[SMX_STRTAB_CAP][24];
    for (int i = 0; i < g_strtab_n; i++) {
        snprintf(lblbuf[i], sizeof lblbuf[i], ".S%d:", g_strtab[i].idx);
        labels[i]  = lblbuf[i];
        escbuf[i]  = gas_escape_str(g_strtab[i].s ? g_strtab[i].s : "");
        escaped[i] = escbuf[i].c_str();
    }
    g_emit.xa_strtab_n       = g_strtab_n;
    g_emit.xa_strtab_labels  = labels;
    g_emit.xa_strtab_escaped = escaped;
    xa_dispatch(XA_STRTAB_RODATA);
    g_emit.xa_strtab_n       = 0;
    g_emit.xa_strtab_labels  = NULL;
    g_emit.xa_strtab_escaped = NULL;
    strtab_reset();
}
#define SMX_CSETTAB_CAP 256
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int sn4_cset32(void) { static int _c = -1; if (_c < 0) { const char * e = getenv("SCRIP_CSET32"); _c = (e && *e == '1') ? 1 : 0; } return _c; }
static struct { const char *s; int idx; unsigned char tbl[256]; unsigned char bits[32]; } g_csettab[SMX_CSETTAB_CAP];
static int g_csettab_n = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void csettab_reset(void) { g_csettab_n = 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const void *csettab_label(char *buf, size_t bufsz, const char *cset)
{
    if (!cset) cset = "";
    for (int i = 0; i < g_csettab_n; i++)
        if (g_csettab[i].s == cset || strcmp(g_csettab[i].s, cset) == 0) { snprintf(buf, bufsz, ".C%d", g_csettab[i].idx); return sn4_cset32() ? (const void *)g_csettab[i].bits : (const void *)g_csettab[i].tbl; }
    if (g_csettab_n >= SMX_CSETTAB_CAP) { fprintf(stderr, "csettab overflow\n"); abort(); }
    int idx = g_csettab_n;
    g_csettab[idx].s = cset; g_csettab[idx].idx = idx;
    memset(g_csettab[idx].tbl, 0, 256); memset(g_csettab[idx].bits, 0, 32);
    for (const unsigned char *p = (const unsigned char *)cset; *p; ++p) { g_csettab[idx].tbl[*p] = 1; g_csettab[idx].bits[*p >> 3] |= (unsigned char)(1u << (*p & 7)); }
    g_csettab_n++;
    snprintf(buf, bufsz, ".C%d", idx);
    return sn4_cset32() ? (const void *)g_csettab[idx].bits : (const void *)g_csettab[idx].tbl;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void xa_emit_csettab_rodata(void)
{
    if (g_csettab_n <= 0) { csettab_reset(); return; }
    static std::string rowbuf[SMX_CSETTAB_CAP];
    static const char *rows[SMX_CSETTAB_CAP];
    static const char *labels[SMX_CSETTAB_CAP];
    static char        lblbuf[SMX_CSETTAB_CAP][24];
    for (int i = 0; i < g_csettab_n; i++) {
        snprintf(lblbuf[i], sizeof lblbuf[i], ".C%d:", g_csettab[i].idx);
        labels[i] = lblbuf[i];
        std::string r;
        if (sn4_cset32()) { for (int j = 0; j < 32; j += 16) { r += " .byte "; for (int k = 0; k < 16; k++) { r += std::to_string((int)g_csettab[i].bits[j + k]); r += (k < 15) ? "," : "\n"; } } }
        else for (int j = 0; j < 256; j += 16) {
            r += " .byte ";
            for (int k = 0; k < 16; k++) { r += std::to_string((int)g_csettab[i].tbl[j + k]); r += (k < 15) ? "," : "\n"; }
        }
        rowbuf[i] = r; rows[i] = rowbuf[i].c_str();
    }
    g_emit.xa_csettab_n      = g_csettab_n;
    g_emit.xa_csettab_labels = labels;
    g_emit.xa_csettab_rows   = rows;
    xa_dispatch(XA_CSETTAB_RODATA);
    g_emit.xa_csettab_n      = 0;
    g_emit.xa_csettab_labels = NULL;
    g_emit.xa_csettab_rows   = NULL;
    csettab_reset();
}
int bb_slot_get(IR_t *nd);
#include "templates/bb_templates.h"
#include "emit_ir.h"
#include "XA.h"
#include "../runtime/builtins/gen.h"
#include "IR.h"
#include "../rt/rt.h"
#include "bb_build.h"
#include "../optimizer/arith_fold.h"
#include "../optimizer/gva_collect.h"
#include "../optimizer/proc_collect.h"
#include "../optimizer/ir_query.h"
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
extern const char * Σ;
extern int          Σlen;
extern atp_t    * bb_atp_new                (const char *varname);
extern cap_t    * bb_cap_new_call           (bb_box_fn child_fn, void *child_state, const char *fnc_name, DESCR_t *fnc_args, int fnc_nargs, char **fnc_arg_names, int fnc_n_arg_names, int immediate);
extern cap_t    * bb_cap_new                (bb_box_fn child_fn, void *child_state, const char *varname, DESCR_t *var_ptr, int immediate);
void (*g_cap_fixup_cb)(void *cap_ptr, const char *child_α_label) = NULL;
const char *child_cache_get_lbl(bb_box_fn fn);
static long flat_buf_max_v() { static long v = -1; if (v < 0) { const char * e = getenv("SCRIP_FLAT_BUF_MB"); long mb = e && *e ? atol(e) : 4; if (mb < 4) mb = 4; if (mb > 256) mb = 256; v = mb * 1024L * 1024L; } return v; }
#define FLAT_BUF_MAX  (flat_buf_max_v())
int g_flat_node_id   = 0;
int g_last_flat_frame_bytes = 0;
int g_last_flat_zstatic = 0;
int g_last_flat_fp = 0;
int g_last_flat_uniform = 0;
extern "C" int zls_g_fp_total(IR_graph_t *);
extern "C" int zls_node_has_fields(const IR_t *);
extern "C" int emit_patzeta_lookup(const char *, int *);
extern "C" void zls_fct_finalize(IR_graph_t *, int);
extern "C" int fc_tail_defer_susp_g(IR_graph_t *, const IR_t *);
extern "C" int fc_tail_dfr(const IR_t *);
extern "C" int sno_name_prologue_bound(const char *);
typedef struct { IR_t *key; int off; } bb_slotmap_ent_t;
static bb_slotmap_ent_t *g_bb_slotmap = NULL;
static int g_bb_slotmap_n = 0;
static int g_bb_slotmap_max = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void bb_slotmap_push(IR_t *nd, int off) {
    if (g_bb_slotmap_n >= g_bb_slotmap_max) {
        int new_max = g_bb_slotmap_max ? g_bb_slotmap_max * 2 : 512;
        bb_slotmap_ent_t *g = (bb_slotmap_ent_t *)realloc(g_bb_slotmap, (size_t)new_max * sizeof(bb_slotmap_ent_t));
        if (!g) return;
        g_bb_slotmap = g; g_bb_slotmap_max = new_max;
    }
    g_bb_slotmap[g_bb_slotmap_n].key = nd; g_bb_slotmap[g_bb_slotmap_n].off = off; g_bb_slotmap_n++;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int bb_slot_get(IR_t *nd) {
    for (int i = 0; i < g_bb_slotmap_n; i++) if (g_bb_slotmap[i].key == nd) return g_bb_slotmap[i].off;
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void bb_slot_register(IR_t *nd, int off) {
    bb_slotmap_push(nd, off);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int bb_varslot_peek(const char *name) {
    return ir_varslot_of(g_emit_cfg, name);
}
int g_proc_direct_active = 0;
int g_gva_active = 0;
int g_gvar_callarg_live = 0;
int g_emit_frame_caller_dl = -1;
int g_frame_active = 0;
int g_flat_outer_nparams = 0;
int g_gen_proc_active = 0;
int g_resumable_callable_active = 0;
int g_scan_regs_live = 0;
static int fbdiv_n = 0; static int fbdiv_mode = -1; static int fbdiv_hooked = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void fbdiv_report(void) { fprintf(stderr, "FB-DIVERGE TOTAL=%d\n", fbdiv_n); }
static int emit_fb_bit_of(IR_t * nd);
static int emit_fb_stmt_scan(IR_graph_t * g);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void emit_fb_divergence_check(void) { if (fbdiv_mode < 0) { const char * e = getenv("SCRIP_FB_DIVERGE"); fbdiv_mode = (e && *e == '1') ? 1 : 0; } if (!fbdiv_mode) return; if (!fbdiv_hooked) { fbdiv_hooked = 1; atexit(fbdiv_report); } if (emit_jmp_pin_legacy() == emit_rec_pin()) return; fbdiv_n++; fprintf(stderr, "FB-DIVERGE #%d graph=%s data_fb=%s rec_fb=%s deep=%d pat=%d gen=%d genproc=%d rescall=%d\n", fbdiv_n, g_emit.flat_lbl_α ? g_emit.flat_lbl_α : "<anon>", emit_jmp_pin_legacy() ? "rsp" : "rsp", emit_rec_pin() ? "rsp" : "rsp", g_emit.flat_deep_arrival, g_emit.flat_pat, g_emit.flat_gen, g_gen_proc_active, g_resumable_callable_active); }
#define FLAT_CHAIN_SET_MAX 512
static IR_t *g_flat_chain_set[FLAT_CHAIN_SET_MAX];
static int   g_flat_chain_set_n = 0;
static bb_label_t *g_limit_gen_beta = NULL;
static bb_label_t *g_scan_body_beta = NULL;
static bb_label_t *g_create_body_entry = NULL;
static bb_label_t *g_move_label_tgt = NULL;
static bb_label_t *g_suspend_dobody_beta = NULL;
static int          g_suspend_resume_slot = -1;
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *child_cache_get_lbl(bb_box_fn fn) { fprintf(stderr, "GROUND ZERO: %s not implemented (Icon-only reset)\n", "child_cache_get_lbl"); abort(); }
int    g_flat_data_any    = 0;
static int    g_flat_data_just_closed = 0;
static char   g_flat_data_pending_lbl[160] = "";
static char   g_flat_data_block_lbls[FLAT_DATA_LBL_MAX][96];
static int    g_flat_data_block_nlbls = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void data_buf_reset(void) { g_flat_data_len = 0; g_flat_data_active = 0; g_flat_data_any = 0; g_flat_data_just_closed = 0; g_flat_data_block_nlbls = 0; g_flat_data_pending_lbl[0] = '\0'; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void data_buf_appendf(const char *fmt, ...) {
    if (g_flat_data_len >= FLAT_DATA_BUF_MAX) return; va_list ap; va_start(ap, fmt);
    int n = vsnprintf(g_flat_data_buf + g_flat_data_len, FLAT_DATA_BUF_MAX - g_flat_data_len, fmt, ap); va_end(ap);
    if (n > 0) { size_t left = FLAT_DATA_BUF_MAX - g_flat_data_len; g_flat_data_len += ((size_t)n < left) ? (size_t)n : left; } }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void data_buf_flush_pending_label(void) { if (!g_flat_data_pending_lbl[0]) return; data_buf_appendf("%s\n", g_flat_data_pending_lbl); g_flat_data_pending_lbl[0] = '\0'; }
#define SYM_SIGMA   "\xCE\xA3"
#define SYM_SIGLEN  "\xCE\xA3""len"
#define SYM_DELTA   "\xCE\x94"
#define ADDR_SIGMA   ((uint64_t)(uintptr_t)&Σ)
#define ADDR_SIGLEN  ((uint64_t)(uintptr_t)&Σlen)
#define ADDR_DELTA   ((uint64_t)(uintptr_t)&Δ)
static const char *(*g_flat_intern_str)(const char *s) = NULL;
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
static bb_label_t g_α_ring[8];
static int        g_α_ring_i = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int        g_bb_alpha_seq = 0;void g_bb_alpha_seq_reset(void) { g_bb_alpha_seq = 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void bb_fill_alpha(IR_t *nd) {
    extern int g_m4_dense_nid;
    bb_label_t *a = &g_α_ring[g_α_ring_i++ & 7];
    if (g_m4_dense_nid) emit_label_initf(a, "bb%d_α", ++g_bb_alpha_seq);
    else                    emit_label_initf(a, "bb%d_α", nd ? bb_node_id(nd) : 0);
    g_emit.lbl_α   = a->name;
    g_emit.lbl_α_p = a;
}
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
static int is_pat_chain_elem(IR_e t);
static int gather_lowered_cat_arms(IR_t *entry, IR_t **arms, IR_t **joins, int cap, IR_t **cat_out, IR_t *stop);
static void gz_emit_cell(IR_t *g, bb_label_t *next_γ, bb_label_t *gw, bb_label_t *gβ, bb_label_t *cut_ω, pl_gz_callee_vec_t *cv);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void gz_emit_catch(IR_t *g, bb_label_t *next_γ, bb_label_t *gw, bb_label_t *gβ, bb_label_t *cut_ω, pl_gz_callee_vec_t *cv) {
    fprintf(stderr, "GROUND ZERO: %s not implemented (Icon-only reset)\n", "gz_emit_catch"); abort(); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void resolve_choice_clause_label(char *dst, size_t dsz, int id, int ci, const char *suffix) {
    fprintf(stderr, "GROUND ZERO: %s not implemented (Icon-only reset)\n", "resolve_choice_clause_label"); abort(); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int bb_kind_is_driver_owned(int t) { fprintf(stderr, "GROUND ZERO: %s not implemented (Icon-only reset)\n", "bb_kind_is_driver_owned"); abort(); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int earn_hazard_in(const IR_t * nd, int depth) {
    if (!nd || depth > 32) return 1;
    if (nd->op == IR_MATCH_ARBNO) return 1;
    if (nd->op == IR_MATCH_DEFER) { const char * _e = getenv("SCRIP_DEFER_HAZ_STATIC"); if (_e && _e[0] == '1' && nd->pat_static) return 0; return 1; }
    if (nd->op == IR_MATCH_VALUE) return 1;
    for (int i = 0; i < nd->n_operands; i++) if (earn_hazard_in(nd->operands[i], depth + 1)) return 1;
    return 0;
}
extern "C" int fc_pair_extent(const IR_t *);
static IR_t * zd_chase(IR_t * t);
static int zd_nops(IR_t * nd);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int alt_arm_member(const IR_t * a, const IR_t * b) {
    if ((!a && !b) || !g_emit_cfg) return 0;
    for (int p = 0; p < g_emit_cfg->n; p++) { IR_t * A = g_emit_cfg->all[p]; if (!A || A->op != IR_MATCH_ALTERNATE) continue;
        int N = (int)(A->n_operands / 2);
        for (int j = 0; j < N; j++) { IR_t * cur = A->operands[2 * j]; IR_t * res = A->operands[2 * j + 1]; int guard = 0;
            while (cur && guard++ <= g_emit_cfg->n) { if ((a && cur == a) || (b && cur == b)) return 1; if (cur == res) break; cur = zd_chase(cur->γ.node); } } }
    return 0;
}
static int arbno_reentry_hazard(const IR_t * nd);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int cap_in_alt_arm(const IR_t * nd) {
    if (!nd || !g_emit_cfg) return 0;
    return alt_arm_member(nd, (nd->n_operands > 1) ? nd->operands[1] : (const IR_t *)0);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int cap_in_repeat_body(const IR_t * nd) {
    if (!nd || !g_emit_cfg) return 0;
    const IR_t * save = (nd->n_operands > 1) ? nd->operands[1] : (const IR_t *)0;
    int ni = -1, si = -1; for (int i = 0; i < g_emit_cfg->n; i++) { if (g_emit_cfg->all[i] == nd) ni = i; if (save && g_emit_cfg->all[i] == save) si = i; }
    for (int a = 0; a < g_emit_cfg->n; a++) { IR_t * C = g_emit_cfg->all[a]; if (!C) continue; int lo = -1, hi = -1;
        if (C->op == IR_MATCH_ARBNO && C->n_operands >= 3) { for (int k = 0; k < g_emit_cfg->n; k++) { if (g_emit_cfg->all[k] == C->operands[1]) lo = k; if (g_emit_cfg->all[k] == C->operands[2]) hi = k; } }
        else if ((C->op == IR_MATCH_FENCE1 || C->op == IR_MATCH_FENCE0) && C->n_operands >= 2) { for (int k = 0; k < g_emit_cfg->n; k++) { if (g_emit_cfg->all[k] == C->operands[0]) lo = k; if (g_emit_cfg->all[k] == C->operands[1]) hi = k; } }
        else continue;
        if (lo < 0 || hi < 0) continue; if (lo > hi) { int t = lo; lo = hi; hi = t; }
        if ((ni >= lo && ni <= hi) || (si >= lo && si <= hi)) return 1; }
    return 0;
}
static int zd_k(IR_t * nd);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int fence_body_kk(const IR_t * nd) {
    if (!nd || !g_emit_cfg || nd->op != IR_MATCH_FENCE1 || nd->n_operands < 2) return 0;
    int lo = -1, hi = -1; for (int k = 0; k < g_emit_cfg->n; k++) { if (g_emit_cfg->all[k] == nd->operands[0]) lo = k; if (g_emit_cfg->all[k] == nd->operands[1]) hi = k; }
    if (lo < 0 || hi < 0) return 0; if (lo > hi) { int t = lo; lo = hi; hi = t; }
    int kk = 0; for (int j = lo; j <= hi; j++) { IR_t * m = g_emit_cfg->all[j]; if (!m || m == nd) continue; int mo = (int)m->op;
        if (mo == IR_MATCH_ALTERNATE || mo == IR_MATCH_ARBNO || (mo == IR_MATCH_FENCE1 || mo == IR_MATCH_FENCE0) || mo == IR_MATCH_DEFER || mo == IR_MATCH_VALUE || mo == IR_CALL || mo == IR_CALL_VALUE || mo == IR_DISJUNCTION || mo == IR_MATCH_ABORT) return 0;
        kk += zd_k(m); }
    return kk;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int fence0_whack_on() { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_FENCE0_WHACK"); v = (e && *e == '0') ? 0 : 1; } return v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int fence0_release_bytes(const IR_t * nd) {
    if (!fence0_whack_on() || !x86_port_cstack()) return 0;
    if (!nd || !g_emit_cfg || nd->op != IR_MATCH_FENCE0) return 0;
    int n = g_emit_cfg->n; if (n <= 0) return 0;
    int _fzd = getenv("SCRIP_FZ_DIAG") ? 1 : 0;
    IR_t ** seq = (IR_t **)alloca(sizeof(IR_t *) * (size_t)n); int ns = -1;
    for (int b = 0; b < n && ns < 0; b++) { IR_t * mb = g_emit_cfg->all[b]; if (!mb || mb->op != IR_MATCH_BEGIN) continue;
        int cnt = 0, guard = 0; IR_t * cur = zd_chase(mb->γ.node);
        while (cur && guard++ <= n) { if (cur == nd) { ns = cnt; break; } if (cnt >= n) break; seq[cnt++] = cur; cur = zd_chase(cur->γ.node); } }
    if (ns < 0) { if (_fzd) fprintf(stderr, "[FZ-DIAG] fence %p not on any MATCH_BEGIN forward chain -- bill 0\n", (const void *)nd); return 0; }
    if (_fzd) fprintf(stderr, "[FZ-DIAG] fence %p executed-prefix len=%d (cfg_n=%d)\n", (const void *)nd, ns, n);
    int total = 0;
    for (int j = ns - 1; j >= 0; j--) { IR_t * m = seq[j]; int mo = (int)m->op;
        if (_fzd) fprintf(stderr, "[FZ-DIAG]   step j=%-3d op=%-22s zd_k=%-3d total=%d\n", j, bb_op_name((IR_e)mo), zd_k(m), total);
        if (mo == IR_MATCH_BEGIN || mo == IR_STATEMENT_BEGIN) break;
        if (mo == IR_MATCH_ALTERNATE || mo == IR_MATCH_ARBNO || mo == IR_MATCH_FENCE1 || mo == IR_MATCH_FENCE0 || mo == IR_MATCH_DEFER || mo == IR_MATCH_VALUE || mo == IR_CALL || mo == IR_CALL_VALUE || mo == IR_DISJUNCTION || mo == IR_MATCH_ABORT) break;
        if (mo == IR_MATCH_SPAN || mo == IR_MATCH_BREAK || mo == IR_MATCH_BREAKX || mo == IR_MATCH_TAB || mo == IR_MATCH_RTAB || mo == IR_MATCH_REM || mo == IR_MATCH_BAL) { total += zd_k(m); continue; }
        if (zd_k(m) == 0) continue;
        break;                         }
    if (_fzd) fprintf(stderr, "[FZ-DIAG] fence %p RELEASE=%d\n", (const void *)nd, total);
    return total;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int fence_frame_candidate(const IR_t * nd) {
    if (!nd || !g_emit_cfg || nd->op != IR_MATCH_FENCE1 || nd->n_operands < 2) return 0;
    if (IR_LIT(nd).ival == 2) return 1;
    int lo = -1, hi = -1; for (int k = 0; k < g_emit_cfg->n; k++) { if (g_emit_cfg->all[k] == nd->operands[0]) lo = k; if (g_emit_cfg->all[k] == nd->operands[1]) hi = k; }
    if (lo < 0 || hi < 0) return 0; if (lo > hi) { int t = lo; lo = hi; hi = t; }
    for (int j = lo; j <= hi; j++) { IR_t * m = g_emit_cfg->all[j]; if (!m || m == nd) continue; int mo = (int)m->op;
        if (mo == IR_MATCH_ALTERNATE || mo == IR_MATCH_ARBNO || (mo == IR_MATCH_FENCE1 || mo == IR_MATCH_FENCE0) || mo == IR_MATCH_DEFER || mo == IR_MATCH_VALUE || mo == IR_CALL || mo == IR_CALL_VALUE || mo == IR_DISJUNCTION || mo == IR_MATCH_ABORT) return 1; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int frame_need_of(const IR_t * nd) {
    if (!nd) return 0;
    switch (nd->op) {
    case IR_MATCH_ARBNO:        return 1;
    case IR_MATCH_FENCE0:       return 1;
    case IR_MATCH_FENCE1:       return 1;
    case IR_MATCH_ASSIGN_SAVE:  {
        if (!g_emit_cfg) return 0;
        for (int _i = 0; _i < g_emit_cfg->n; _i++) { IR_t * _c = g_emit_cfg->all[_i];
            if (_c && (_c->op == IR_MATCH_ASSIGN_COND || _c->op == IR_MATCH_ASSIGN_IMM) && _c->n_operands > 1 && _c->operands[1] == nd) { int _old = earn_hazard_in(_c, 0); int _new = frame_need_of(_c); if (getenv("SCRIP_CAP_NEST_DIAG") && _old != _new) fprintf(stderr, "[CAPNEST-SAVE-DIVERGE] c=%p old_haz=%d new_fno=%d\n", (void*)_c, _old, _new); return _new; } }
        return 0; }
    case IR_MATCH_ASSIGN_IMM:
    case IR_MATCH_ASSIGN_COND:  {
        int h = earn_hazard_in(nd, 0);
        static int _cn = -1; if (_cn < 0) { const char * e = getenv("SCRIP_CAP_NEST"); _cn = (e && *e == '0') ? 0 : 1; }
        if (_cn && !h && g_emit_cfg && nd->n_operands > 1 && nd->operands[1]) {
            int _si = -1, _ci = -1;
            for (int _i = 0; _i < g_emit_cfg->n; _i++) { if (g_emit_cfg->all[_i] == nd->operands[1]) _si = _i; if (g_emit_cfg->all[_i] == nd) _ci = _i; }
            int _ext = fc_pair_extent(nd);
            if (getenv("SCRIP_CAP_NEST_DIAG")) fprintf(stderr, "[CAPNEST] nd=%p save=%p cfg_n=%d si=%d ci=%d ext=%d\n", (void*)nd, (void*)nd->operands[1], g_emit_cfg->n, _si, _ci, _ext);
            if (_si >= 0 && _ci >= 0 && _ext > 0) { int _lo = (_si > _ci ? _si : _ci) + 1, _hi = _ext < g_emit_cfg->n ? _ext : g_emit_cfg->n;
                for (int _i = _lo; _i < _hi && !h; _i++) { IR_t * _m = g_emit_cfg->all[_i];
                    if (getenv("SCRIP_CAP_NEST_DIAG")) fprintf(stderr, "[CAPNEST]   scan[%d] op=%d pat_static=%d\n", _i, _m ? (int)_m->op : -1, _m ? _m->pat_static : -1);
                    if (_m && (_m->op == IR_MATCH_ARBNO || (_m->op == IR_MATCH_DEFER && !_m->pat_static) || _m->op == IR_MATCH_VALUE)) h = 1; } }
        }
        if (!h) h = cap_in_alt_arm(nd);
        if (!h) h = cap_in_repeat_body(nd);
        return h; }
    default:                    return earn_hazard_in(nd, 0);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void bb_prepare(IR_t *nd) {
    if (!PLATFORM_X86) return;
    g_emit.bb_ls = NULL;
    g_emit.bb_rs = NULL;
    g_emit.bb_op_lbl = NULL;
    g_emit.bb_lk = -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void bb_classify_node(IR_t *nd) {
    if (!PLATFORM_X86) return;
    g_emit.op_frame_need = frame_need_of(nd);
    { static int _ed = -1; if (_ed < 0) { const char * e = getenv("SCRIP_EARN_DIAG"); _ed = (e && *e == '1') ? 1 : 0; }
      if (_ed && nd) fprintf(stderr, "[EARN] op=%d need=%d haz=%d pat_static=%d n_op=%d\n", nd->op, g_emit.op_frame_need, earn_hazard_in(nd, 0), nd->pat_static, nd->n_operands); }
    { static int _cd = -1; if (_cd < 0) { const char * e = getenv("SCRIP_CLASS_DIAG"); _cd = (e && *e == '1') ? 1 : 0; }
      if (_cd && nd) { extern const char * bb_op_name(IR_e);
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
    if (binop_is_concat((long)op))        return BINOP_CAT_CONCAT;
    if (op == BINOP_XREP)                 return BINOP_CAT_XREP;
    if (op == BINOP_ADD || op == BINOP_SUB || op == BINOP_MUL || op == BINOP_DIV || op == BINOP_MOD || op == BINOP_POW) return BINOP_CAT_ARITH;
    if (op == BINOP_CUNION || op == BINOP_CDIFF || op == BINOP_CINTER) return BINOP_CAT_ARITH;
    return BINOP_CAT_ARITH;
}
extern "C" int zls_off(const IR_t *);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int nd_slot(const IR_t *nd) { return nd ? zls_off(nd) : -1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int emit_binop_opnd_slot(IR_t *o) {
    if (!o) return -1;
    extern int is_global(const char *);
    if (o->op == IR_VAR && IR_LIT(o).sval && IR_LIT(o).sval[0] != '&' && (!is_global(IR_LIT(o).sval) || graph_has_local(g_emit_cfg, IR_LIT(o).sval))) { int voff = bb_varslot_peek(IR_LIT(o).sval); if (voff >= 0) return voff; }
    int s = bb_slot_get(o); if (s >= 0) return s;
    return nd_slot(o);
}
static int binop_operand_real_static(IR_graph_t *g, IR_t *o, int depth);
static IR_t *g_bnr_vnode[4096]; static int g_bnr_vn = 0; static const char *g_bnr_vname[4096]; static int g_bnr_vnn = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int bnr_node_seen(IR_t *o) { for (int i = 0; i < g_bnr_vn; i++) if (g_bnr_vnode[i] == o) return 1; if (g_bnr_vn < 4096) g_bnr_vnode[g_bnr_vn++] = o; return 0; }
static int bnr_name_seen(const char *n) { for (int i = 0; i < g_bnr_vnn; i++) if (g_bnr_vname[i] == n || (g_bnr_vname[i] && !strcmp(g_bnr_vname[i], n))) return 1; if (g_bnr_vnn < 4096) g_bnr_vname[g_bnr_vnn++] = n; return 0; }
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
    if (op == BINOP_POW) return 1;
    int is_num = (op == BINOP_ADD || op == BINOP_SUB || op == BINOP_MUL || op == BINOP_DIV || op == BINOP_MOD || (op >= BINOP_LT && op <= BINOP_NE));
    if (!is_num) return 0;
    g_bnr_vn = 0; g_bnr_vnn = 0;
    int r = binop_operand_real_static(g, bb_child0(nd), 0); if (!r) { g_bnr_vn = 0; g_bnr_vnn = 0; r = binop_operand_real_static(g, bb_child1(nd), 0); }
    return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int to_inner_gen_operand_k(IR_t *gi, IR_t **nodes, int n) {
    int bk = -1;
    if (gi->op != IR_TO && gi->op != IR_TO_BY) return -1;
    for (int oi = 0; oi < gi->n_operands; oi++) for (int k = 0; k < n; k++) if (nodes[k] == (IR_t *)gi->operands[oi] && ir_is_generator_kind(nodes[k]->op) && k > bk) bk = k;
    return bk;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int bb_call_write_route(IR_t *nd) {
    extern int g_icon_write_reassignable;
    const char *fn = IR_LIT(nd).sval; int64_t narg = IR_LIT(nd).ival; IR_t *a0 = ir_call_arg(nd, 0);
    if (!(fn && narg == 1 && a0 && !strcmp(fn, "write"))) return 0;
    if (g_icon_write_reassignable) return 0;
    { IR_t *_gt = nd->γ.node; int _gg = 0; while (_gt && _gt->op == IR_GOTO && _gg++ < 128) _gt = _gt->γ.node; IR_t *_ot = nd->ω.node; int _og = 0; while (_ot && _ot->op == IR_GOTO && _og++ < 128) _ot = _ot->γ.node; if (g_emit_cfg && g_emit_cfg->icn_cells_graph && _gt && _ot && _gt == _ot && (unsigned char)nd->ω.sz[0] == 0xce && (unsigned char)nd->ω.sz[1] == 0xb2) return 0; }
    if (bb_slot_get(a0) >= 0) return 1;
    int wintexpr = (a0->op == IR_BINOP || a0->op == IR_LIT_INTEGER || a0->op == IR_TO || a0->op == IR_TO_BY || a0->op == IR_VAR || a0->op == IR_CALL || ir_is_call_kind(a0->op));
    if (wintexpr && (a0->op == IR_BINOP || a0->op == IR_TO || a0->op == IR_TO_BY)) return (a0->op == IR_BINOP && binop_is_concat((long)IR_LIT(a0).ival)) ? 2 : 3;
    if (wintexpr) return 4;
    if (a0->op == IR_LIT_STRING && IR_LIT(a0).sval) return 5;
    return 0;
}
extern "C" int icn_builtin_is_known(const char *);
extern "C" int icn_builtin_is_generator(const char *);
extern "C" int pl_builtin_is_known(const char *);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int bb_call_route_classify(IR_t * nd) {
    const char * fn = g_emit.op_sval ? g_emit.op_sval : ""; int64_t narg = g_emit.op_ival; IR_t * a0 = ir_call_arg(nd, 0); double dv = g_emit.op_dval;
    IR_e k = nd ? nd->op : IR_CALL;
    if (k == IR_CALL_BUILTIN_GEN) return CALL_ROUTE_BYNAME_GEN;
    if (k == IR_CALL_BUILTIN && fn[0] && rt_builtin_is_generator(fn)) return CALL_ROUTE_BYNAME;
    if (k == IR_CALL_PROC_STAGED) return CALL_ROUTE_PROC_STAGED;
    if (k == IR_CALL_BUILTIN && g_emit.op_write_route == 0 && fn[0] && rt_builtin_is_known(fn)) return CALL_ROUTE_FN;
    if (k == IR_CALL_BUILTIN_ICON && fn[0] && icn_builtin_is_generator(fn)) return CALL_ROUTE_BYNAME;
    if (k == IR_CALL_BUILTIN_ICON && fn[0] && icn_builtin_is_known(fn)) return CALL_ROUTE_FN;
    if (k == IR_CALL_BUILTIN_PROLOG && fn[0] && pl_builtin_is_known(fn)) return CALL_ROUTE_FN;
    if (!strcmp(fn, "__rk_bool") && narg >= 1) return CALL_ROUTE_RK_BOOL_SLOT;
    if (dv == 2.0 && fn[0] && rt_builtin_is_known(fn)) return CALL_ROUTE_BYNAME;
    if (dv == 2.0 && !strcmp(fn, "__rk_bool")) return CALL_ROUTE_RK_BOOL_COND;
    if ((dv == 2.0 || dv == 3.0) && fn[0] && rt_proc_is_registered(fn) && rt_proc_is_generator(fn)) return CALL_ROUTE_PROC_STAGED;
    if ((dv == 2.0 || dv == 3.0) && fn[0] && rt_builtin_is_generator(fn)) return CALL_ROUTE_BYNAME;
    if (dv == 2.0) return CALL_ROUTE_DVAL2_BOMB;
    if (fn[0] && rt_proc_is_registered(fn)) return CALL_ROUTE_PROC_STAGED;
    if (!strcmp(fn, "__rk_bool") && dv == 0.0 && narg == 1 && a0 && bb_slot_get(a0) >= 0) return CALL_ROUTE_RK_BOOL_SLOT;
    switch (g_emit.op_write_route) {
    case 1: return CALL_ROUTE_WRITE_SLOT; case 2: case 3: return CALL_ROUTE_WRITE_BINOP;
    case 4: return CALL_ROUTE_WRITE_LEGACY; case 5: return CALL_ROUTE_WRITE_EMPTY; default: break; }
    if (fn[0] && rt_builtin_is_known(fn)) return CALL_ROUTE_FN;
    if (fn[0] && !rt_proc_is_registered(fn)) return CALL_ROUTE_BYNAME;
    return CALL_ROUTE_FATAL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void walk_bb_flat(IR_t *nd, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) { fprintf(stderr, "GROUND ZERO: %s not implemented (Icon-only reset)\n", "walk_bb_flat"); abort(); }
extern "C" int zls2_geom(const IR_t *, int, int *, long *);
extern "C" int fc_geom(const IR_t *, long *);
extern "C" int fc_cond_fp(const IR_t *);
extern "C" int fc_head_fp(const IR_t *);
extern "C" int fc_leaf_disp(const IR_t *);
extern "C" int fc_seq_active(const IR_t *);
extern "C" int fc_call_active(const IR_t *);
extern "C" int zls_arbno_geom(const IR_t *, int *, int *);
extern "C" int zls_arbno_zq(const IR_t *, int *, int);
static int walk_bb_node_inner(IR_t * nd, FILE * out);
extern "C" int bbprof_on(void);
extern "C" void bbprof_record(int nid, int kind, int uid, void *lo, void *hi);
int g_zprobe_depth = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int walk_bb_node(IR_t * nd, FILE * out) {
    { static int _zp = -1; if (_zp < 0) { const char * e = getenv("SCRIP_ZPROBE"); _zp = (e && *e == '1') ? 1 : 0; }
      if (_zp && nd) { extern int fc_geom(const IR_t *, long *); extern long zw_node_k(const IR_t *); long _d; long _k = fc_geom(nd, &_d) ? -1 : zw_node_k(nd);
          fprintf(stderr, "[ZPROBE] nest=%d nid=%d op=%d k=%ld\n", g_zprobe_depth, bb_node_id(nd), (int)nd->op, _k);
          g_zprobe_depth++; int _rc = walk_bb_node_inner(nd, out); g_zprobe_depth--; return _rc; } }
    if (!(nd && MEDIUM_BINARY && bbprof_on())) return walk_bb_node_inner(nd, out);
    int lo = bb_emit_pos;
    int rc = walk_bb_node_inner(nd, out);
    if (bb_emit_pos > lo) bbprof_record(bb_node_id(nd), (int)nd->op, g_emit.x86_uid, (void *)(bb_emit_buf + lo), (void *)(bb_emit_buf + bb_emit_pos));
    return rc;
}
static int drive_value_slot(IR_t *nd);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int emit_sep_on(void) { static int _sep = -1; if (_sep < 0) { const char *e = getenv("SCRIP_ASM_SEP"); _sep = (e && *e == '0') ? 0 : 1; } return _sep; }
static void emit_sep_rule(char ch) { if (emit_sep_on()) bb_emit_x86(x86("commentrule", std::string(119, ch))); }
extern "C" void emit_sep_rule_c(char ch) { emit_sep_rule(ch); }
static int zw_nid_listed(const char * e, int nid) { if (!e || !*e) return 0; const char * p = e; while (*p) { long v = strtol(p, (char **)&p, 10); if ((int)v == nid) return 1; while (*p && *p != ',') p++; if (*p) p++; } return 0; }
static int g_zd_stage, g_zd_arm, g_zd_k, g_zd_gpop, g_zd_wpop, g_zd_wsteal, g_zd_read[6], g_zd_kind[6], g_zd_ztail, g_zd_zunder, g_zd_zpat, g_zd_zfc;
static int g_arbk16_stmt = 0;
static long g_zd_suspend_uclaim = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int zd_sr_role(const IR_t * nd) { int64_t v = IR_LIT(nd).ival; return (v == 1 || v == 2 || v == 3 || v == 4) ? (int)v : 0; }
static int zd_k(IR_t * nd);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int cap_ok_at(IR_t * base, int bi) { for (int _j = 0; _j < g_emit_cfg->n; _j++) { IR_t * _o = g_emit_cfg->all[_j]; if (_o && _o->op == IR_MATCH_ARBNO && _o->n_operands >= 3) { int _a = -1, _b = -1; for (int _k = 0; _k < g_emit_cfg->n; _k++) { if (g_emit_cfg->all[_k] == _o->operands[1]) _a = _k; if (g_emit_cfg->all[_k] == _o->operands[2]) _b = _k; } if (_a > _b) { int _t = _a; _a = _b; _b = _t; } if (_a >= 0 && bi >= _a && bi <= _b) return 0; } } (void)base; return 1; }
static int cap_anchor_of(IR_t * nd) { (void)nd; return 0; }
static const char * sr3_gamma_label(const IR_t * g) { static int _off = -1; if (_off < 0) { const char * e = getenv("SCRIP_SR3_FIX"); _off = (e && *e == '0') ? 1 : 0; } if (getenv("SCRIP_SR3_CENSUS")) fprintf(stderr, "SR3 gamma op=%d\n", (int)g->op); if (!_off && g->op != IR_GOTO_DEFERRED) return (const char *)0; return IR_LIT(g).sval; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int walk_bb_node_inner(IR_t * nd, FILE * out) {
    { extern int zls_result_live(const IR_t *); g_emit.op_res_live = zls_result_live(nd); }
    { static int _ba = -1; static const char * _bo; static const char * _bs; if (_ba < 0) { const char * e = getenv("SCRIP_BB_ALLOC"); _ba = (e && *e == '0') ? 0 : 1; _bo = getenv("SCRIP_BB_ONLY"); _bs = getenv("SCRIP_BB_SKIP"); }
      int _spine = (nd->op == IR_BINOP || nd->op == IR_ASSIGN || nd->op == IR_LIT_INTEGER || nd->op == IR_LIT_STRING || nd->op == IR_LIT_REAL || nd->op == IR_LIT_CHARSET || nd->op == IR_LIT_NAME || nd->op == IR_VAR || nd->op == IR_CMP_TEST || nd->op == IR_COERCE_NUMERIC || nd->op == IR_IDENT || nd->op == IR_DIFFER);
      { static int _all = -1; if (_all < 0) { const char * e = getenv("SCRIP_BB_ALLOC_ALL"); _all = (e && *e == '1') ? 1 : 0; }
      (void)_ba; (void)_bo; (void)_bs; (void)_all; (void)_spine;
      { extern long zw_carve_k(const IR_t *); extern int zc_nofc(void); static int _kc = -1; if (_kc < 0) { const char * e = getenv("SCRIP_NOFC_CARVE"); _kc = (e && *e == '1') ? 1 : 0; } long _k = (zc_nofc() && !_kc) ? 0 : zw_carve_k(nd);
        if (_k > 0) g_emit.op_fc_bytes = _k; } } }
    g_emit.op_zdepth = x86_fc_on() ? (int)g_emit.op_fc_bytes : 0;
    { extern int zdp_alpha(const IR_t *); extern int zdp_beta(const IR_t *); extern int zdp_known_alpha(const IR_t *); extern int zdp_known_beta(const IR_t *); extern int zdp_mode(void);
      g_emit.op_zdp_ad = (nd && zdp_mode() && zdp_known_alpha(nd)) ? zdp_alpha(nd) : -1;
      g_emit.op_zdp_bd = (nd && zdp_mode() && zdp_known_beta(nd))  ? zdp_beta(nd)  : -1;
      g_emit.op_zdp_rbp = (nd && (nd->op == IR_MATCH_BEGIN || nd->op == IR_MATCH_DEFER)) ? 1 : 0; }
    g_emit.op_pair_rejoin = (nd && nd->op == IR_MATCH_ALTERNATE) ? 1 : 0;
    g_emit.op_stmt_dyn = g_arbk16_stmt; if (nd) { if (nd->op == IR_STATEMENT_BEGIN) { g_arbk16_stmt = 0; g_emit.op_stmt_dyn = 0; } else if (nd->op == IR_STATEMENT_END || nd->op == IR_STATEMENT) g_arbk16_stmt = 0; }
    g_emit.op_zres = 0; g_emit.op_zgpop = 0; g_emit.op_wsteal = 0; g_emit.op_ztail = 0; g_emit.op_zpat = 0; g_emit.op_zfc = 0; g_emit.op_xf_off = -1; for (int _zi = 0; _zi < 6; _zi++) { g_emit.op_zread[_zi] = 0; g_emit.op_zkind[_zi] = -1; g_emit.op_zread_xf[_zi] = -1; }
    if (nd) { g_emit.op_xf_off = xop_frame_slot(nd); int _xn = zd_nops(nd); if (_xn > 6) _xn = 6; for (int _zi = 0; _zi < _xn && _zi < nd->n_operands; _zi++) g_emit.op_zread_xf[_zi] = xop_frame_slot(nd->operands[_zi]); }
    g_emit.op_suspend_stmt_uclaim = 0;
    if (g_zd_stage) { g_emit.op_suspend_stmt_uclaim = g_zd_suspend_uclaim; g_zd_suspend_uclaim = 0;
        if (g_zd_arm) { g_emit.op_zres = 1; g_emit.op_fc_bytes = g_zd_k; g_emit.op_fc_base = -1; g_emit.op_zdepth = g_zd_k + g_zd_zunder;       for (int _zi = 0; _zi < 6; _zi++) { g_emit.op_zread[_zi] = g_zd_read[_zi]; g_emit.op_zkind[_zi] = g_zd_kind[_zi]; } g_emit.op_ztail = g_zd_ztail; g_zd_ztail = 0; g_emit.op_zpat = g_zd_zpat; g_zd_zpat = 0; g_emit.op_zfc = g_zd_zfc; g_zd_zfc = 0;    }
        g_emit.op_zgpop = (g_emit.flat_stmt_frame || (g_emit.flat_jmp_entry && g_emit.flat_pat)) ? 0 : g_zd_gpop; g_emit.op_wpop += g_zd_wpop; g_emit.op_wsteal = g_zd_wsteal; g_zd_stage = 0; }
    extern void bb_prepare_capture_arbno(IR_t *nd, int imm);
    extern void bb_prepare(IR_t *nd);
    extern int  bb_slot_get(IR_t *nd);
    if (!nd) return 1;
    g_emit.node = nd;
    emit_set_sink(out);
    g_emit.sid  = 0;
    g_emit.nid  = bb_node_id(nd);
    g_emit.x86_uid = g_flat_node_id++;
    g_emit.frame_region = g_emit_cfg ? ((32 + g_emit_cfg->jcon_value_region + 15) & ~15) : 0;
    g_emit.op_sval = (nd->op == IR_VAR || nd->op == IR_VAR_REF || nd->op == IR_ASSIGN || nd->op == IR_LIT_STRING || nd->op == IR_LIT_CHARSET || nd->op == IR_LIT_NAME
                       || nd->op == IR_KEYWORD_ICON || nd->op == IR_KEYWORD_ICON_GEN || nd->op == IR_KEYWORD_SNOBOL4 || nd->op == IR_KEYWORD_ASSIGN || nd->op == IR_KEYWORD_ASSIGN_SNOBOL4 || nd->op == IR_REV_SWAP || nd->op == IR_FIELD_GET || nd->op == IR_FIELD_VAR || nd->op == IR_SUBSCRIPT || nd->op == IR_ITERATE
                       || nd->op == IR_MATCH_ASSIGN_COND || nd->op == IR_MATCH_ASSIGN_SAVE || nd->op == IR_MATCH_ASSIGN_IMM || nd->op == IR_MATCH_LIT || nd->op == IR_MATCH_ANY || nd->op == IR_MATCH_NOTANY || nd->op == IR_MATCH_SPAN
                       || nd->op == IR_MATCH_BREAK || nd->op == IR_MATCH_BREAKX || nd->op == IR_MATCH_DEFER || nd->op == IR_MATCH_ATP || nd->op == IR_MATCH_REPLACE || nd->op == IR_GOTO_DEFERRED
                       || nd->op == IR_NULLTEST_VAR || nd->op == IR_PROC_GEN || nd->op == IR_PROC_VALUE || ir_norm_call_kind(nd->op) == IR_CALL)
                    ? IR_LIT(nd).sval : (const char *)0;
    { extern int g_gva_active; extern int gva_index_of(const char *); int nm_op = (nd->op == IR_VAR || nd->op == IR_VAR_REF || nd->op == IR_ASSIGN);
      int defer_op = ((nd->op == IR_MATCH_DEFER) && IR_LIT(nd).sval && IR_LIT(nd).sval[0] != '*');
      g_emit.op_gva_k = (g_gva_active && (nm_op || defer_op) && IR_LIT(nd).sval && !graph_has_local(g_emit_cfg, IR_LIT(nd).sval)) ? gva_index_of(IR_LIT(nd).sval) : -1; }
    { extern int g_proc_direct_active; extern int proc_slot_of(const char *); extern int proc_direct_eligible(const char *); int nm_op = (ir_norm_call_kind(nd->op) == IR_CALL);
      g_emit.op_proc_k = (g_proc_direct_active && nm_op && IR_LIT(nd).sval && proc_direct_eligible(IR_LIT(nd).sval)) ? proc_slot_of(IR_LIT(nd).sval) : -1; }
    if (nd->op == IR_STATEMENT_BEGIN || nd->op == IR_STATEMENT_END || nd->op == IR_STATEMENT) g_emit.op_stno = (int32_t)IR_LIT(nd).ival;
    g_emit.op_ival = (ir_norm_call_kind(nd->op) == IR_CALL || nd->op == IR_PROC_GEN) ? (int64_t)nd->n_operands : (nd->op == IR_GOTO_DEFERRED ? (int64_t)nd->seal : IR_LIT(nd).ival);
    if (nd->op == IR_DEFINE && ir_define_sr_citizen(nd)) { int64_t r = (int64_t)zd_sr_role(nd); g_emit.op_ival = r; g_emit.op_sval = r ? (const char *)0 : IR_LIT(nd).sval; }
    g_emit.op_node_kind = (int)nd->op;
    g_emit.op_cap_anchor = 0;
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
    { static int bm = -1; if (bm < 0) { const char *e = getenv("SCRIP_BLOB_MAP"); bm = (e && *e == '1') ? 1 : 0; } if (bm && MEDIUM_BINARY) { extern const char *bb_op_name(IR_e); fprintf(stderr, "BLOBBOX %p %s n%d\n", (void *)(bb_emit_buf + bb_emit_pos), bb_op_name(nd->op), g_emit.x86_uid); } }
    { static int _sc = -1; if (_sc < 0) { const char *e = getenv("SCRIP_SRC_COMMENT"); _sc = (e && *e == '0') ? 0 : 1; } g_emit.op_src = _sc ? bb_src_of(nd) : (const char *)0; }
    { if (g_emit.op_src && *g_emit.op_src) emit_sep_rule('=');
      if (g_emit.op_src) { std::string _c; const char * p = g_emit.op_src; while (*p) { const char * e2 = p; while (*e2 && *e2 != '\n') e2++; _c += x86("srccomment", std::string(p, (size_t)(e2 - p))); p = *e2 ? e2 + 1 : e2; } bb_emit_x86(_c); }
      emit_sep_rule('-'); }
    bb_classify_node(nd);
    if (emit_floater_kind(nd) == 3) { bb_emit_x86(bb_nreturn_mark()); return 0; }
    switch (nd->op) {
    case IR_LIT_INTEGER:
    case IR_LIT_STRING:
    case IR_LIT_CHARSET:
    case IR_LIT_NAME:
    case IR_LIT_REAL:               { { long fck; if (!g_emit.op_zres && fc_geom(nd, &fck)) { g_emit.op_fc_bytes = fck; g_emit.op_fc_base = g_emit.op_off; } } bb_emit_x86(bb_lit_scalar()); }         return 0;
    case IR_KEYWORD_ICON:
    case IR_KEYWORD_ICON_GEN:     bb_emit_x86(bb_keyword_icon());       return 0;
    case IR_KEYWORD_SNOBOL4:      bb_emit_x86(bb_keyword_snobol4());    return 0;
    case IR_KEYWORD_ASSIGN:       bb_emit_x86(bb_keyword_assign());     return 0;
    case IR_KEYWORD_ASSIGN_SNOBOL4: bb_emit_x86(bb_keyword_assign_snobol4()); return 0;
    case IR_VAR:                  { extern int is_global(const char *);
        const char * _vn = IR_LIT(nd).sval;
        int _vn_reassignable_builtin = _vn && (!strcmp(_vn, "write") || !strcmp(_vn, "writes"));
        if (_vn && _vn[0] == '&') bb_emit_x86(bb_keyword_icon());
        else if (_vn && ((is_global(_vn) && !graph_has_local(g_emit_cfg, _vn)) || _vn_reassignable_builtin)) { { long fck; if (!g_emit.op_zres && fc_geom(nd, &fck)) { g_emit.op_fc_bytes = fck; g_emit.op_fc_base = g_emit.op_off; } } { extern long fc_vwpop(const IR_t *); long _w = fc_vwpop(nd); if (_w > 0 && !g_emit.op_zres) g_emit.op_wpop += (int)_w; } bb_emit_x86(bb_var_global()); }
        else bb_emit_x86(bb_var()); } return 0;
    case IR_VAR_REF:              { extern int is_global(const char *); const char * _rn = IR_LIT(nd).sval;
        if (_rn && is_global(_rn) && !graph_has_local(g_emit_cfg, _rn)) { g_emit.op_sa = -1; g_emit.op_gva_k = g_gva_active ? gva_index_of(_rn) : -1; }
        else if (_rn) { int _vo = bb_varslot_peek(_rn); g_emit.op_sa = _vo; g_emit.op_gva_k = -1; }
        bb_emit_x86(bb_var_ref()); } return 0;
    case IR_COERCE_STRING:        { bb_prepare(nd); bb_emit_x86(bb_coerce_string()); }  return 0;
    case IR_COERCE_INTEGER:       { bb_prepare(nd); bb_emit_x86(bb_coerce_integer()); } return 0;
    case IR_COERCE_NUMERIC:       { bb_prepare(nd); bb_emit_x86(bb_coerce_numeric()); } return 0;
    case IR_COERCE_REAL:          { bb_prepare(nd); bb_emit_x86(bb_coerce_real()); }    return 0;
    case IR_CMP_TEST:             { bb_prepare(nd); bb_emit_x86(bb_cmp_test()); }      return 0;
    case IR_IDENT:                { bb_prepare(nd); bb_emit_x86(bb_ident()); }         return 0;
    case IR_DIFFER:               { bb_prepare(nd); bb_emit_x86(bb_differ()); }        return 0;
    case IR_ASSIGN: {
        extern int is_global(const char *);
        const char * _an = IR_LIT(nd).sval;
        int _an_reassignable_builtin = _an && (!strcmp(_an, "write") || !strcmp(_an, "writes"));
        if (_an && ((is_global(_an) && !graph_has_local(g_emit_cfg, _an)) || _an_reassignable_builtin)) { bb_emit_x86(bb_assign_global()); return 0; }
        if (_an) { { static int _dd = -1; if (_dd < 0) { const char *_e = getenv("SCRIP_DRIVE_DIAG"); _dd = (_e && _e[0] == '1') ? 1 : 0; }
                     if (_dd && !(g_emit.op_sb >= 0 && g_emit.op_off >= 0 && (g_emit.op_a_slot >= 0 || g_emit.op_a_node_kind == (int)IR_OP_COUNT))) {
                         IR_t *_ra = (nd->n_operands > 0) ? nd->operands[0] : (IR_t *)0;
                         fprintf(stderr, "[ASSIGN-DIAG] var=%s sb=%d off=%d a_slot=%d a_kind=%d rhs_op=%s rhs_nops=%d\n", _an, g_emit.op_sb, g_emit.op_off, g_emit.op_a_slot, g_emit.op_a_node_kind, _ra ? bb_op_name(_ra->op) : "NULL", _ra ? _ra->n_operands : -1); } }
                   bb_emit_x86(bb_assign_local()); return 0; }
        fprintf(out, "# [walk_bb_node: kind=%d unhandled]\n", (int)nd->op); return 1;
    }
    case IR_BINOP_TEST:          bb_emit_x86(g_emit.op_zres ? bb_binop_relop_val() : bb_binop_relop()); return 0;
    case IR_BINOP_RELOP_VAL:     bb_emit_x86(bb_binop_relop_val());   return 0;
    case IR_BINOP:
        { extern int fc_vbinop_active(const IR_t *); extern long fc_vwpop(const IR_t *); long _w = fc_vwpop(nd); if (_w > 0 && fc_vbinop_active(nd) && !g_emit.op_zres) g_emit.op_wpop += (int)_w; }
        switch (g_emit.op_binop_kind) {
        case BINOP_CAT_RELOP:  bb_emit_x86(bb_binop_relop());       return 0;
        case BINOP_CAT_CONCAT: bb_emit_x86(bb_binop_concat_slot()); return 0;
        case BINOP_CAT_XREP:   bb_emit_x86(bb_binop_xrep_slot());   return 0;
        case BINOP_CAT_ARITH:
        default:              bb_emit_x86(bb_binop_arith());       return 0;
        }
    case IR_SUCCEED:              bb_emit_x86(bb_succeed());        return 0;
    case IR_SUSPEND:              bb_emit_x86(bb_suspend());        return 0;
    case IR_TO:                   { bb_prepare(nd); bb_emit_x86(bb_to()); } return 0;
    case IR_MATCH_LEN:            { bb_prepare(nd); { const char * _sv = (nd->n_operands == 0 && (uintptr_t)(uint64_t)IR_LIT(nd).ival > (uintptr_t)0xFFFFU) ? IR_LIT(nd).sval : (const char *)0; g_emit.op_sval = (_sv && _sv[0] == '*') ? _sv : (const char *)0; } bb_emit_x86(bb_match_len()); } return 0;
    case IR_MATCH_LIT:            { bb_prepare(nd); bb_emit_x86(bb_match_lit()); } return 0;
    case IR_MATCH_ANY:            { bb_prepare(nd); bb_emit_x86(bb_match_any()); } return 0;
    case IR_MATCH_NOTANY:         { bb_prepare(nd); bb_emit_x86(bb_match_notany()); } return 0;
    case IR_MATCH_SPAN:           { bb_prepare(nd); { long fck; if (fc_geom(nd, &fck)) { g_emit.op_fc_bytes = fck; g_emit.op_fc_base = g_emit.x86_scratch_off; } } bb_emit_x86(bb_match_span()); } return 0;
    case IR_MATCH_BREAK:          { bb_prepare(nd); { long fck; if (fc_geom(nd, &fck)) { g_emit.op_fc_bytes = fck; g_emit.op_fc_base = g_emit.x86_scratch_off; } } bb_emit_x86(bb_match_break()); } return 0;
    case IR_MATCH_BREAKX:         { bb_prepare(nd); { long fck; if (fc_geom(nd, &fck)) { g_emit.op_fc_bytes = fck; g_emit.op_fc_base = g_emit.x86_scratch_off; } } bb_emit_x86(bb_match_breakx()); } return 0;
    case IR_MATCH_TAB:            { bb_prepare(nd); { long fck; if (fc_geom(nd, &fck)) { g_emit.op_fc_bytes = fck; g_emit.op_fc_base = g_emit.x86_scratch_off; } } bb_emit_x86(bb_match_tab()); } return 0;
    case IR_MATCH_RTAB:           { bb_prepare(nd); { long fck; if (fc_geom(nd, &fck)) { g_emit.op_fc_bytes = fck; g_emit.op_fc_base = g_emit.x86_scratch_off; } } bb_emit_x86(bb_match_rtab()); } return 0;
    case IR_MATCH_POS:            { bb_prepare(nd); bb_emit_x86(bb_match_pos()); } return 0;
    case IR_MATCH_RPOS:           { bb_prepare(nd); bb_emit_x86(bb_match_rpos()); } return 0;
    case IR_MATCH_REM:            { bb_prepare(nd); { long fck; if (fc_geom(nd, &fck)) { g_emit.op_fc_bytes = fck; g_emit.op_fc_base = g_emit.x86_scratch_off; } } bb_emit_x86(bb_match_rem()); } return 0;
    case IR_MATCH_ATP:            { bb_prepare(nd); bb_emit_x86(bb_match_atp()); } return 0;
    case IR_MATCH_ARB:            { bb_prepare(nd); { long fck; if (fc_geom(nd, &fck)) { g_emit.op_fc_bytes = fck; g_emit.op_fc_base = g_emit.x86_scratch_off; } } bb_emit_x86(bb_match_arb()); } return 0;
    case IR_MATCH_BAL:            { bb_prepare(nd); { long fck; if (fc_geom(nd, &fck)) { g_emit.op_fc_bytes = fck; g_emit.op_fc_base = g_emit.x86_scratch_off; } } bb_emit_x86(bb_match_bal()); } return 0;
    case IR_MATCH_DEFER: { bb_prepare(nd); g_emit.op_seal = nd->seal; g_emit.op_off = drive_value_slot(nd); g_emit.op_defer_leaf_susp = g_emit_cfg ? fc_tail_defer_susp_g(g_emit_cfg, nd) : -1; bb_emit_x86(bb_match_defer()); } return 0;
    case IR_MATCH_VALUE:          { bb_prepare(nd); bb_emit_x86(bb_match_value()); } return 0;
    case IR_MATCH_ARBNO:          { bb_prepare(nd); g_emit.op_arbno_rbp = emit_arbno_rbp();
                                    g_emit.op_arbno_frame_off = arbno_frame_slot(nd);
                                    extern int fc_tail_arbno(const IR_t *, int *, int *, int *, int *); extern int fc_tail_ncap(const IR_t *); int _fpb = 0, _fpl = 0, _osb = 0, _hdr = 0;
                                    int _tailc = (x86_zc_frame() == ZC_FRAME_RSP && fc_tail_arbno(nd, &_fpb, &_fpl, &_osb, &_hdr)) ? 1 : 0;
                                    int _k16r = 0; { static int _k16e = -1; if (_k16e < 0) { const char * _e1 = getenv("SCRIP_ARBNO_K16"); const char * _e2 = getenv("SCRIP_ARBNO_FRAMELESS"); _k16e = ((_e1 ? (atoi(_e1) != 0) : 1) && (_e2 ? (atoi(_e2) != 0) : 1)) ? 1 : 0; } if (_k16e && (x86_port_mode() == ZC_PORT_FORTH || x86_port_mode() == ZC_PORT_HEAP) && g_emit_cfg && nd->n_operands >= 3) { int _ni=-1; for(int _j=0;_j<g_emit_cfg->n;_j++) if(g_emit_cfg->all[_j]==nd){_ni=_j;break;} int _fr=0; if(_ni>=0) for(int _j=0;_j<g_emit_cfg->n&&!_fr;_j++){IR_t*_o=g_emit_cfg->all[_j]; if(_o&&_o!=nd&&_o->op==IR_MATCH_ARBNO&&_o->n_operands>=3){int _a=-1,_b=-1; for(int _k=0;_k<g_emit_cfg->n;_k++){if(g_emit_cfg->all[_k]==_o->operands[1])_a=_k;if(g_emit_cfg->all[_k]==_o->operands[2])_b=_k;} if(_a>_b){int _t=_a;_a=_b;_b=_t;} if(_a>=0&&_ni>=_a&&_ni<=_b)_fr=1;}} int _k0=1,_kk=0,_sq=1; { int _s0=-1,_s1=-1; for(int _j=0;_j<g_emit_cfg->n;_j++){if(g_emit_cfg->all[_j]==nd->operands[1])_s0=_j;if(g_emit_cfg->all[_j]==nd->operands[2])_s1=_j;} if(_s0>_s1){int _t=_s0;_s0=_s1;_s1=_t;} if(_s0<0){_k0=0;_sq=0;} else for(int _j=_s0;_j<=_s1;_j++){IR_t*_m=g_emit_cfg->all[_j]; if(_m&&_m!=nd){int _mo2=(int)_m->op; if(_mo2==IR_MATCH_ALTERNATE||_mo2==IR_MATCH_ARBNO||(_mo2==IR_MATCH_FENCE1 || _mo2==IR_MATCH_FENCE0)||_mo2==IR_MATCH_DEFER||_mo2==IR_MATCH_VALUE||_mo2==IR_CALL||_mo2==IR_CALL_VALUE||_mo2==IR_DISJUNCTION||_mo2==IR_MATCH_ABORT)_sq=0; int _mk=zd_k(_m); if(_mk!=0)_k0=0; _kk+=_mk;}} } int _osv=0,_dmb=-2,_dme=-2; { int _mb=-1,_hi=g_emit_cfg->n; for(int _j=_ni;_j>=0;_j--){if(g_emit_cfg->all[_j]&&g_emit_cfg->all[_j]->op==IR_MATCH_BEGIN){_mb=_j;break;}} if(_mb>=0) for(int _j=_mb+1;_j<g_emit_cfg->n;_j++){if(g_emit_cfg->all[_j]&&g_emit_cfg->all[_j]->op==IR_MATCH_BEGIN){_hi=_j;break;}} if(_mb>=0){ int _s0=-1,_s1=-1; for(int _j=0;_j<g_emit_cfg->n;_j++){if(g_emit_cfg->all[_j]==nd->operands[1])_s0=_j;if(g_emit_cfg->all[_j]==nd->operands[2])_s1=_j;} if(_s0>_s1){int _t=_s0;_s0=_s1;_s1=_t;} for(int _j=_mb+1;_j<_hi;_j++){ if(_j>=_s0&&_j<=_s1) continue; IR_t*_m=g_emit_cfg->all[_j]; if(_m&&_m->op==IR_MATCH_ASSIGN_SAVE)_osv=1; } } else _osv=1; _dmb=_mb; _dme=_hi; }    if (_sq && !_k0 && _kk > 0 && !_fr && !_osv) { _k16r = 1; g_arbk16_stmt = 1; g_emit.op_arbno_framed = 0; g_emit.op_arbno_body_k0 = 0; g_emit.op_arbno_body_kk = _kk; }       { static int _ad=-1; if(_ad<0){const char*_e=getenv("SCRIP_ARBNO_DIAG"); _ad=(_e&&*_e=='1')?1:0;} if(_ad) fprintf(stderr,"[ARBNO-K16] framed=%d k0=%d sq=%d kk=%d osv=%d mb=%d me=%d route=%s\n", _fr, _k0, _sq, _kk, _osv, _dmb, _dme, _k16r?"FRAMELESS_K":"legacy"); } } }
                                    if (!_k16r && 0 && _tailc) { g_emit.op_tail = 1;    g_emit.op_sb = _osb; g_emit.op_sa = _hdr; g_emit.op_tail_fpb = _fpb; g_emit.op_tail_fpl = _fpl; g_emit.op_tail_ncap = fc_tail_ncap(nd); g_emit.op_tail_seal = (nd->n_operands > 3 && nd->operands[3] == nd) ? 1 : 0; g_emit.op_tail_dfr = fc_tail_dfr(nd); { extern int fc_tail_fpr_rsp(const IR_t *); g_emit.op_tail_fpr_rsp = fc_tail_fpr_rsp(nd); } }
                                    else if (!_k16r) { int _mo = -1, _sp = 0; if (zls_arbno_geom(nd, &_mo, &_sp)) { g_emit.op_sa = _mo; int _chain = ((x86_port_mode() == ZC_PORT_FORTH || x86_port_mode() == ZC_PORT_HEAP)); g_emit.op_arbno_chain = _chain; g_emit.op_sb = (_sp + (_chain ? 24 : 16) + 15) & ~15; if (_chain) { g_emit.op_arbno_nzq = zls_arbno_zq(nd, g_emit.op_arbno_zq, 8); if (_chain && g_emit_cfg && nd->n_operands >= 3) { int _i0=-1,_i1=-1; for(int _j=0;_j<g_emit_cfg->n;_j++){if(g_emit_cfg->all[_j]==nd->operands[1])_i0=_j;if(g_emit_cfg->all[_j]==nd->operands[2])_i1=_j;} if(_i0>_i1){int _t=_i0;_i0=_i1;_i1=_t;} int _bha=0; for(int _j=_i0;_j<=_i1&&_j>=0&&!_bha;_j++) if(g_emit_cfg->all[_j]&&g_emit_cfg->all[_j]->op==IR_MATCH_ARBNO&&g_emit_cfg->all[_j]!=nd) _bha=1; g_emit.op_body_has_arbno=_bha; } }
                                           if (_chain && g_emit_cfg) { int _ni=-1; for(int _j=0;_j<g_emit_cfg->n;_j++) if(g_emit_cfg->all[_j]==nd){_ni=_j;break;} int _fr=0; if(_ni>=0) for(int _j=0;_j<g_emit_cfg->n&&!_fr;_j++){IR_t*_o=g_emit_cfg->all[_j]; if(_o&&_o!=nd&&_o->op==IR_MATCH_ARBNO&&_o->n_operands>=3){int _a=-1,_b=-1; for(int _k=0;_k<g_emit_cfg->n;_k++){if(g_emit_cfg->all[_k]==_o->operands[1])_a=_k;if(g_emit_cfg->all[_k]==_o->operands[2])_b=_k;} if(_a>_b){int _t=_a;_a=_b;_b=_t;} if(_a>=0&&_ni>=_a&&_ni<=_b)_fr=1;}} g_emit.op_arbno_framed=_fr; int _k0=1; { int _s0=-1,_s1=-1; for(int _j=0;_j<g_emit_cfg->n;_j++){if(g_emit_cfg->all[_j]==nd->operands[1])_s0=_j;if(g_emit_cfg->all[_j]==nd->operands[2])_s1=_j;} if(_s0>_s1){int _t=_s0;_s0=_s1;_s1=_t;} if(_s0<0)_k0=0; else for(int _j=_s0;_j<=_s1&&_k0;_j++){IR_t*_m=g_emit_cfg->all[_j]; if(_m&&_m!=nd){int _mo3=(int)_m->op; if(zd_k(_m)!=0||_mo3==IR_MATCH_DEFER||_mo3==IR_MATCH_ALTERNATE||_mo3==IR_MATCH_ARBNO||(_mo3==IR_MATCH_FENCE1 || _mo3==IR_MATCH_FENCE0)||_mo3==IR_MATCH_VALUE||_mo3==IR_CALL||_mo3==IR_CALL_VALUE||_mo3==IR_DISJUNCTION||_mo3==IR_MATCH_ABORT)_k0=0;}} } g_emit.op_arbno_body_k0=_k0; }
   { int _du=0; int _ds0=-1,_ds1=-1; for(int _j=0;_j<g_emit_cfg->n;_j++){if(g_emit_cfg->all[_j]==nd->operands[1])_ds0=_j;if(g_emit_cfg->all[_j]==nd->operands[2])_ds1=_j;} if(_ds0>_ds1){int _t=_ds0;_ds0=_ds1;_ds1=_t;} int _alt=0,_ref=0,_dfr=0,_arb=0,_altnest=0; if(_ds0>=0) for(int _j=_ds0;_j<=_ds1;_j++){IR_t*_m=g_emit_cfg->all[_j]; if(_m&&_m!=nd&&(int)_m->op==IR_MATCH_DEFER&&!_m->pat_static)_du=1; if(!_m||_m==nd)continue; int _o=(int)_m->op; if(_o==IR_MATCH_ALTERNATE||_o==IR_DISJUNCTION){ if(_m->seal)_ref=1; else { _alt++; for(int _q=0;_q<_m->n_operands;_q++){IR_t*_a=_m->operands[_q]; if(_a&&((int)_a->op==IR_MATCH_ALTERNATE||(int)_a->op==IR_DISJUNCTION))_altnest=1;} } continue;} if(_o==IR_MATCH_DEFER){ extern int sn4_alt_carrier(void); if(sn4_alt_carrier()&&_m->seal!=1)_dfr++; else _ref=1; continue;} if(_o==IR_MATCH_ARBNO){ extern int sn4_arbno_tailbeta(void); if(sn4_arbno_tailbeta()&&_m->γ.node==nd)_arb++; else _ref=1; continue;}       switch(_o){case IR_MATCH_LIT:case IR_MATCH_LEN:case IR_MATCH_ANY:case IR_MATCH_NOTANY:case IR_MATCH_POS:case IR_MATCH_RPOS:case IR_MATCH_TAB:case IR_MATCH_RTAB:case IR_MATCH_SPAN:case IR_MATCH_BREAK:case IR_MATCH_REM:break;default:_ref=1;break;} } int _fseal=(nd->n_operands>3&&nd->operands[3]==nd)?1:0; g_emit.op_tail_seal=_fseal;    g_emit.op_arbno_body_defer_unsafe=_du||arbno_reentry_hazard(nd); { extern int sn4_arbno_altsib(void); int _altok=sn4_arbno_altsib()?(_alt>=1&&!_altnest):(_alt==1); g_emit.op_arbno_body_actframe=(((_altok&&_dfr==0&&_arb==0)||(_alt==0&&_dfr==1&&_arb==0)||(_alt==0&&_dfr==0&&_arb==1))&&!_ref&&!_fseal)?0:1; } }                   } else { g_emit.op_sa = -1; g_emit.op_sb = -1; g_emit.op_arbno_chain = 0; } }
                                    if (g_emit.op_arbno_chain && g_emit.op_off >= 0 && !(nd->n_operands > 3 && nd->operands[3] == nd) && g_emit_cfg && nd->n_operands >= 3) {
                                        static int _dtl = -1; if (_dtl < 0) { const char * _e = getenv("SCRIP_ARBNO_LATCH"); _dtl = _e ? (atoi(_e) != 0) : 0; }
                                        if (_dtl) {
                                            int _i0 = -1, _i1 = -1; for (int _j = 0; _j < g_emit_cfg->n; _j++) { if (g_emit_cfg->all[_j] == nd->operands[1]) _i0 = _j; if (g_emit_cfg->all[_j] == nd->operands[2]) _i1 = _j; }
                                            if (_i0 > _i1) { int _t = _i0; _i0 = _i1; _i1 = _t; }
                                            IR_t * _dn = NULL; int _ok = (_i0 >= 0 && _i1 >= 0);
                                            for (int _j = _i0; _ok && _j <= _i1; _j++) { IR_t * _x = g_emit_cfg->all[_j]; if (!_x) continue;
                                                if (_x->op == IR_MATCH_DEFER) { if (_dn || _x->seal != 2) _ok = 0; else _dn = _x; }
                                                else if (_x->op == IR_LIT_STRING || _x->op == IR_GOTO) { }
                                                else _ok = 0; }
                                            const char * _pn = NULL;
                                            if (_ok && _dn) for (int _j = 0; _j < _dn->n_operands; _j++) { IR_t * _o = _dn->operands[_j]; if (_o && _o->op == IR_LIT_STRING && IR_LIT(_o).sval && !strncmp(IR_LIT(_o).sval, "PAT$", 4)) { _pn = IR_LIT(_o).sval; break; } }
                                            int _susp = 0;
                                            static int _pk = -1; if (_pk < 0) { const char * _e2 = getenv("SCRIP_ARBNO_PEEK"); _pk = _e2 ? (atoi(_e2) != 0) : 1; }
                                            if (getenv("SCRIP_DT_DIAG")) fprintf(stderr, "[DT] arbno: ok=%d pn=%s var=%s pro=%d uni=%d pk=%d\n", _ok, _pn ? _pn : "-", (_dn && IR_LIT(_dn).sval) ? IR_LIT(_dn).sval : "-", (_dn && IR_LIT(_dn).sval) ? sno_name_prologue_bound(IR_LIT(_dn).sval) : -1, _pn ? emit_patzeta_lookup(_pn, &_susp) : -1, _pk);
                                            if (_ok && _dn && IR_LIT(_dn).sval && sno_name_prologue_bound(IR_LIT(_dn).sval) && (_pk ? 1 : (_pn && emit_patzeta_lookup(_pn, &_susp) != 0))) { g_emit.op_arbno_dt = 1; g_emit.op_arbno_dt_susp = _pk ? 0 : _susp; }
                                        }
                                    }
                                    bb_emit_x86(bb_match_arbno()); } return 0;
    case IR_MATCH_BEGIN:           { extern int fc_tail_head(const IR_t *); int _th = (x86_zc_frame() == ZC_FRAME_RSP && fc_tail_head(nd)); if (fc_head_fp(nd) >= 0 || _th) { g_emit.op_fc_wbytes = 80; g_emit.op_fc_base = g_emit.op_off; } if (_th) g_emit.op_tail = 1;       bb_emit_x86(bb_match_begin()); } return 0;
    case IR_MATCH_END:        { IR_t * _hd = nd->n_operands > 0 ? nd->operands[0] : (IR_t *)0; extern int fc_tail_release(const IR_t *, int *); int _br = -1;
                                    { int _tr = (x86_zc_frame() == ZC_FRAME_RSP && _hd) ? fc_tail_release(_hd, &_br) : 0; int _fp = _hd ? fc_head_fp(_hd) : -999;
                                      if (getenv("SCRIP_FCDISP_DIAG")) fprintf(stderr, "[FCDISP] tail_release=%d br=%d head_fp=%d\n", _tr, _br, _fp); }
                                    if (x86_zc_frame() == ZC_FRAME_RSP && _hd && fc_tail_release(_hd, &_br)) { g_emit.op_tail = 1; g_emit.op_fc_disp = _br; }
                                    else g_emit.op_fc_disp = _hd ? fc_head_fp(_hd) : -1;
                                    { extern int fc_tail_head(const IR_t *); if (_hd && (fc_head_fp(_hd) >= 0 || fc_tail_head(_hd)) && x86_zc_frame() == ZC_FRAME_RSP) { g_emit.op_fc_wbytes = 80; g_emit.op_fc_base = g_emit.op_off; } }
                                    bb_emit_x86(bb_match_end()); } return 0;
    case IR_MATCH_REPLACE:        { bb_emit_x86(bb_match_replace()); } return 0;
    case IR_MATCH_ASSIGN_COND:    { bb_prepare(nd); g_emit.op_fc_disp = fc_cond_fp(nd); g_emit.op_cap_anchor = cap_anchor_of(nd); g_emit.op_cap_frame_off = capture_frame_slot(nd); if (getenv("SCRIP_CAP_DIAG")) fprintf(stderr, "[CAP] COND nd=%p fc_disp=%d port=%d zres=%d zread0=%d anchor=%d frame_off=%d\n", (void*)nd, g_emit.op_fc_disp, (int)x86_port_mode(), g_emit.op_zres, g_emit.op_zread[0], g_emit.op_cap_anchor, g_emit.op_cap_frame_off); bb_emit_x86(bb_match_capture()); } return 0;
    case IR_MATCH_ASSIGN_IMM:     { bb_prepare(nd); g_emit.op_fc_disp = fc_cond_fp(nd); g_emit.op_cap_anchor = cap_anchor_of(nd); g_emit.op_cap_frame_off = capture_frame_slot(nd); if (getenv("SCRIP_CAP_DIAG")) fprintf(stderr, "[CAP] IMM nd=%p fc_disp=%d anchor=%d off=%d pin=%d refine=%d deep=%d pat=%d frame_off=%d\n", (void*)nd, g_emit.op_fc_disp, g_emit.op_cap_anchor, g_emit.op_off, emit_jmp_pin_legacy(), g_emit.flat_fb_refine, g_emit.flat_deep_arrival, g_emit.flat_pat, g_emit.op_cap_frame_off); bb_emit_x86(bb_match_capture()); } return 0;
    case IR_MATCH_ASSIGN_SAVE:    { bb_prepare(nd); g_emit.op_cap_anchor = cap_anchor_of(nd); g_emit.op_cap_frame_off = capture_frame_slot(nd); { long fck; if (fc_geom(nd, &fck)) { g_emit.op_fc_bytes = fck; g_emit.op_fc_base = g_emit.op_cap_anchor ? -1 : g_emit.op_off; } } { extern int fc_save_active(const IR_t *); if (getenv("SCRIP_CAP_DIAG")) fprintf(stderr, "[CAP] SAVE nd=%p save_active=%d fc_bytes=%ld port=%d anchor=%d frame_off=%d\n", (void*)nd, fc_save_active(nd), (long)g_emit.op_fc_bytes, (int)x86_port_mode(), g_emit.op_cap_anchor, g_emit.op_cap_frame_off); } bb_emit_x86(bb_match_capture()); } return 0;
    case IR_MATCH_ALTERNATE:      { bb_emit_x86(bb_match_alternate()); } return 0;
    case IR_MATCH_FENCE0:          { bb_prepare(nd); g_emit.op_fence0_release = fence0_release_bytes(nd); bb_emit_x86(bb_match_fence0()); } return 0;
    case IR_MATCH_FENCE1:          { bb_prepare(nd); g_emit.op_fence_body_kk = fence_body_kk(nd); g_emit.op_fence_frame_off = fence_frame_slot(nd); bb_emit_x86(bb_match_fence1()); } return 0;
    case IR_MATCH_ABORT:           { bb_emit_x86(bb_match_abort()); } return 0;
    case IR_SCAN_SEQUENCE:        { bb_emit_x86(bb_scan_sequence()); } return 0;
    case IR_SCAN_ALTERNATE:       { bb_emit_x86(bb_scan_alternate()); } return 0;
    case IR_TO_BY:                { bb_prepare(nd); bb_emit_x86(bb_to_by()); } return 0;
    case IR_MAKE_LIST:            bb_emit_x86(bb_make_list());      return 0;
    case IR_CONJUNCTION:                 bb_emit_x86(bb_conjunction());           return 0;
    case IR_GOTO:                 { static int _mgt = -1; if (_mgt < 0) _mgt = getenv("MONITOR_GOTO_TAP") ? 1 : 0; extern void emit_mon_label_tap(int32_t); extern int g_monitor_bin; if (g_monitor_bin && _mgt && g_emit.op_stno > 0) emit_mon_label_tap(g_emit.op_stno); } bb_emit_x86(bb_goto());           return 0;
    case IR_GOTO_DEFERRED:             bb_emit_x86(bb_goto_deferred());       return 0;
    case IR_DEFINE: {
        if (ir_define_sr_citizen(nd)) { extern int fc_call_active(const IR_t *); if (g_emit.op_ival == 4 && nd->n_operands >= 2 && nd->operands[0] && nd->operands[1]) { g_emit.op_sval = IR_LIT(nd->operands[0]).sval; g_emit.lbl_t0 = IR_LIT(nd->operands[1]).sval; }    if (g_emit.op_ival == 3 && nd->γ.node && sr3_gamma_label(nd->γ.node)) { const char *_e = sr3_gamma_label(nd->γ.node); g_emit.op_sval = strncmp(_e, "LBL__", 5) ? _e : _e + 5; }    if (g_emit.op_ival == 0 && nd->γ.node && fc_call_active(nd->γ.node) && nd->n_operands == 1 && nd->operands[0]) { extern int zls_off(const IR_t *); int _as = bb_slot_get(nd->operands[0]); if (_as < 0) _as = zls_off(nd->operands[0]); if (_as >= 0) { g_emit.op_fc_wbytes = 16; g_emit.op_fc_base = _as; } } g_emit.op_define_role = (int)g_emit.op_ival; extern std::string bb_define(); bb_emit_x86(bb_define()); return 0; }
        if (nd->n_operands == 0) { g_emit.op_sval = IR_LIT(nd).sval;    g_emit.lbl_t0 = (const char *)0; if (g_emit_cfg && g_emit_cfg->n_dentry > 0) for (int _dq = 0; _dq < g_emit_cfg->n_dentry; _dq++) if (g_emit_cfg->dentry_node[_dq] == nd) { g_emit.lbl_t0 = g_emit_cfg->dentry_name[_dq]; break; }    g_emit.op_define_role = 6; extern std::string bb_define(); bb_emit_x86(bb_define()); int _d1st = 1; if (g_emit_cfg && g_emit.op_sval) for (int _dr = 0; _dr < g_emit_cfg->n_dentry; _dr++) { IR_t *_dp = g_emit_cfg->dentry_node[_dr]; if (_dp == nd) break; if (_dp && _dp->n_operands == 0 && IR_LIT(_dp).sval && !strcmp(IR_LIT(_dp).sval, g_emit.op_sval)) { _d1st = 0; break; } }    if (g_is_text && g_emit.lbl_t0 && _d1st) { long _sv5 = g_emit.op_ival; g_emit.op_ival = 5; g_emit.op_define_role = 5; bb_emit_x86(bb_define()); g_emit.op_ival = _sv5; }    return 0; }
        g_emit.op_sval = IR_LIT(nd).sval;
        g_emit.op_ival = (long)nd->n_operands;
        g_emit.op_ab_nformals = nd->seal;
        { extern int g_gva_active; int _ab_n = (int)nd->n_operands; drive_arg_slots_reserve(_ab_n);
          for (int _k = 0; _k < _ab_n; _k++) { const char * _nm = nd->operands[_k] ? IR_LIT(nd->operands[_k]).sval : (const char *)0; g_emit.op_arg_slot[_k] = (_nm && g_gva_active) ? gva_index_of(_nm) : -1; } g_emit.op_arg_slot_n = _ab_n; }
        g_emit.op_define_role = 7;
        extern std::string bb_define();
        bb_emit_x86(bb_define());
    } return 0;
    case IR_STATEMENT_BEGIN:      { extern int g_monitor_bin; extern int g_mon_max_stno; g_emit.op_mon_stmt_tap = ((g_monitor_bin || x86_zdp_rbp_on()) && g_emit.op_stno > 0) ? 1 : 0;    if (g_emit.op_mon_stmt_tap && g_emit.op_stno > g_mon_max_stno) g_mon_max_stno = g_emit.op_stno; g_emit.op_fc_bytes = 0; bb_emit_x86(bb_statement()); { extern std::string bb_zdp_anchor(long, long); static int _zdpa = -1; if (_zdpa < 0) { const char * e = getenv("SCRIP_ZDP_TEARDOWN"); _zdpa = (e && *e == '1') ? 1 : 0; } if (_zdpa) bb_emit_x86(bb_zdp_anchor((long)nd->op, (long)bb_node_id((IR_t *)nd))); }    g_emit.op_mon_stmt_tap = 0; } return 0;
    case IR_STATEMENT_END:
    case IR_STATEMENT:            { g_emit.op_fc_bytes = 0; bb_emit_x86(bb_statement()); } return 0;
    case IR_BOUND:                { g_emit.op_sb = 1; g_emit.op_off = zls_off(nd); g_emit.op_fc_bytes = 0; bb_emit_x86(bb_bound()); } return 0;
    case IR_UNMARK:               { IR_t * _mk = nd->n_operands > 0 ? nd->operands[0] : (IR_t *)0; g_emit.op_sb = 0; g_emit.op_off = _mk ? zls_off(_mk) : -1; g_emit.op_fc_bytes = 0; bb_emit_x86(bb_bound()); } return 0;
    case IR_SUBSCRIPT:            bb_emit_x86(nd->n_operands == 2 ? bb_subscript() : bb_section()); return 0;
    case IR_DEREF:                bb_emit_x86(bb_deref());          return 0;
    case IR_RANDOM:               bb_emit_x86(bb_random());         return 0;
    case IR_ASSIGN_VAR:           bb_emit_x86(bb_assign_var());     return 0;
    case IR_REV_ASSIGN:                bb_emit_x86(bb_rev_assign());          return 0;
    case IR_REV_SWAP:                  bb_emit_x86(bb_rev_swap());            return 0;
    case IR_REV_ASSIGN_VAR:            bb_emit_x86(bb_rev_assign_var()); return 0;
    case IR_SWAP:                 bb_emit_x86(bb_swap());           return 0;
    case IR_SWAP_VAR:             bb_emit_x86(bb_swap_var());       return 0;
    case IR_PROC_VALUE:           bb_emit_x86(bb_proc_value());     return 0;
    case IR_CALL_VALUE:           bb_emit_x86(bb_call_value());     return 0;
    case IR_LIMIT:                bb_emit_x86(bb_limit());          return 0;
    case IR_REPALT:                return 0;
    case IR_ITERATE:              bb_emit_x86(bb_iterate());        return 0;
    case IR_SCAN_ENTER:           { g_emit.op_sb = 1; g_emit.op_sa = g_emit.op_a_slot; bb_emit_x86(bb_gen_scan()); } return 0;
    case IR_INITIAL:           bb_emit_x86(bb_enter_init());     return 0;
    case IR_CREATE:                bb_emit_x86(bb_create());        return 0;
    case IR_ACTIVATE:              bb_emit_x86(bb_activate());      return 0;
    case IR_CORET:                 bb_emit_x86(bb_coret());         return 0;
    case IR_COFAIL:                bb_emit_x86(bb_cofail());        return 0;
    case IR_MOVE_LABEL:            bb_emit_x86(bb_move_label());    return 0;
    case IR_DISJUNCTION: { if (nd->n_operands > 0) { bb_emit_x86(bb_disjunction()); return 0; }
                                                  bb_emit_x86(bb_indirect_goto()); } return 0;
    case IR_SCAN:                 { IR_t *_en = (nd->n_operands > 0) ? nd->operands[0] : NULL; IR_t *_bv = (nd->n_operands > 1) ? nd->operands[1] : NULL; g_emit.op_sb = 0; g_emit.op_off = nd_slot(_en); g_emit.op_sa = _bv ? nd_slot(_bv) : -1; g_emit.op_ival = zls_off(nd); g_emit.lbl_t0 = g_scan_body_beta ? g_scan_body_beta->name : NULL; g_emit.lbl_t0_p = g_scan_body_beta; bb_emit_x86(bb_gen_scan()); } return 0;
    case IR_SCAN_TAB:             { long fck; if (fc_geom(nd, &fck) && getenv("SCRIP_ICN_FCCARVE")) { g_emit.op_fc_bytes = fck; g_emit.op_fc_base = g_emit.op_off + 16; if (g_emit.op_sa >= 0 && !getenv("SCRIP_NO_CM_CARVE")) g_emit.op_sa += (int)fck; } bb_emit_x86(bb_scan_tab()); }    return 0;
    case IR_SCAN_MOVE:            { long fck; if (fc_geom(nd, &fck) && getenv("SCRIP_ICN_FCCARVE")) { g_emit.op_fc_bytes = fck; g_emit.op_fc_base = g_emit.op_off + 16; if (g_emit.op_sa >= 0 && !getenv("SCRIP_NO_CM_CARVE")) g_emit.op_sa += (int)fck; } bb_emit_x86(bb_scan_move()); }   return 0;
    case IR_SCAN_UPTO:            bb_emit_x86(bb_scan_upto());   return 0;
    case IR_SCAN_ANY:             bb_emit_x86(bb_scan_any());    return 0;
    case IR_SCAN_MANY:            bb_emit_x86(bb_scan_many());   return 0;
    case IR_SCAN_FIND:            bb_emit_x86(bb_scan_find());   return 0;
    case IR_SCAN_MATCH:           { long fck; if (fc_geom(nd, &fck) && getenv("SCRIP_ICN_FCCARVE")) { g_emit.op_fc_bytes = fck; g_emit.op_fc_base = g_emit.op_off + 16; } bb_emit_x86(bb_scan_match()); }  return 0;
    case IR_SCAN_POS:             bb_emit_x86(bb_scan_pos());    return 0;
    case IR_SCAN_BAL:             bb_emit_x86(bb_scan_bal());    return 0;
    case IR_GLIT:                 { g_emit.op_name1 = IR_LIT(nd).sval; bb_emit_x86(bb_glit()); } return 0;
    case IR_GCC:                  { g_emit.op_name1 = IR_LIT(nd).sval; bb_emit_x86(bb_gcc());  } return 0;
    case IR_GALT:                 { g_emit.op_off = 0; bb_emit_x86(bb_galt()); }                 return 0;
    case IR_RETURN: {
        IR_t *rv = (nd->n_operands > 0 && nd->operands[0]) ? nd->operands[0] : (IR_t *)0;
        g_emit.op_sa = rv ? bb_slot_get(rv) : -1; g_emit.op_dval = IR_LIT(nd).dval;
        if (g_emit.flat_gen && g_suspend_resume_slot >= 0) { g_emit.op_sb = g_suspend_resume_slot; g_emit.lbl_t1_p = g_emit.flat_fail_p; g_emit.lbl_t1 = g_emit.flat_fail_p ? g_emit.flat_fail_p->name : (const char *)0; }
        else { g_emit.op_sb = -1; g_emit.lbl_t1_p = (bb_label_t *)0; g_emit.lbl_t1 = (const char *)0; }
        bb_emit_x86(bb_return()); return 0; }
    case IR_CALL_PROC_STAGED: case IR_CALL_BUILTIN: case IR_CALL_BUILTIN_GEN:
    case IR_CALL_BUILTIN_ICON: case IR_CALL_BUILTIN_SNOBOL4: case IR_CALL_BUILTIN_PROLOG:
    case IR_PROC_GEN:
    case IR_CALL: {
        g_emit.op_call_route = bb_call_route_classify(nd);
        { extern int fc_call_active(const IR_t *); extern long fc_vwpop(const IR_t *); extern int zls_off(const IR_t *); if (fc_call_active(nd)) { g_emit.op_fc_wbytes = 16; g_emit.op_fc_base = zls_off(nd); long _w = fc_vwpop(nd); if (_w > 0) g_emit.op_wpop += (int)_w; } }
        bb_emit_x86(bb_call(nd));
        return 0;
    }
    case IR_FAIL:            bb_emit_x86(bb_fail());                            return 0;
    case IR_CUT:             bb_emit_x86(bb_cut());                             return 0;
    case IR_UNOP:
    case IR_UNOP_TEST: case IR_NULLTEST_VAR: {
        { extern int fc_vbinop_active(const IR_t *); extern long fc_vwpop(const IR_t *); long _w = fc_vwpop(nd); if (_w > 0 && fc_vbinop_active(nd) && !g_emit.op_zres) g_emit.op_wpop += (int)_w; }
        bb_emit_x86(bb_unop());   return 0; }
    case IR_FIELD_GET: case IR_FIELD_VAR: bb_emit_x86(bb_field_get());   return 0;
    default:
        fprintf(out, "# [walk_bb_node: kind=%d unhandled]\n", (int)nd->op);
        return 1;
    }
}
extern int           bb_slot_get(IR_t *nd);
extern void          bb_slot_register(IR_t *nd, int off);
extern int           bb_varslot_peek(const char *name);
extern int           is_global(const char *name);
extern int           zls_result_live(const IR_t *nd);
extern int           gva_index_of(const char *name);
extern int           g_gva_active;
extern IR_graph_t *  g_emit_cfg;
#define DRIVE_FILL(nd,a,s,f,b) do { \
    g_emit.op_zls2_bytes = 0; g_emit.op_zls2_slot = -1; g_emit.op_zls2_ops = 0; g_emit.op_selfload = 0; \
    g_emit.op_fc_bytes = 0; g_emit.op_fc_base = -1; g_emit.x86_fc_synth = 240; g_emit.op_fc_fpmax = -1; g_emit.op_fc_disp = -1; g_emit.op_fc_wbytes = 0; g_emit.op_arbno_chain = 0; g_emit.op_arbno_nzq = 0; g_emit.op_tail = 0; g_emit.op_tail_fpb = 0; g_emit.op_tail_fpl = 0; g_emit.op_tail_seal = 0; g_emit.op_tail_ncap = 0; g_emit.op_arbno_dt = 0; g_emit.op_arbno_dt_susp = 0; g_emit.op_defer_leaf_susp = -1; g_emit.op_tail_dfr = 0; g_emit.op_tail_fpr_rsp = 0; g_emit.op_body_has_arbno = 0; g_emit.op_arbno_framed = 0; g_emit.op_arbno_body_k0 = 0; g_emit.op_arbno_body_kk = 0; g_emit.op_fence_body_kk = 0; g_emit.op_fence_frame_off = -1; g_emit.op_arbno_body_defer_unsafe = 0; g_emit.op_arbno_body_actframe = 0; g_emit.op_cap_anchor = 0; g_emit.op_arbno_rbp = 0; \
     \
    bb_label_t *_fs__=bb_label_fold((s)), *_ff__=bb_label_fold((f)); \
    g_emit.lbl_α=(a)->name; g_emit.lbl_γ=_fs__->name; g_emit.lbl_ω=_ff__->name; g_emit.lbl_β=(b)->name; \
    g_emit.lbl_α_p=(a); g_emit.lbl_γ_p=_fs__; g_emit.lbl_ω_p=_ff__; g_emit.lbl_β_p=(b); \
    walk_bb_node((nd), emit_outf()); } while(0)
#define DRIVE_PAIR_RESET()      do { g_emit.xa_bb_emit_pair_n = 0; } while(0)
#define DRIVE_PAIR_JMP(tgt)     do { int _i=g_emit.xa_bb_emit_pair_n++; g_emit.xa_bb_emit_pair_define[_i]=NULL; g_emit.xa_bb_emit_pair_jmp[_i]=(tgt); } while(0)
#define DRIVE_PAIR_DEF_JMP(l,t) do { int _i=g_emit.xa_bb_emit_pair_n++; g_emit.xa_bb_emit_pair_define[_i]=(l); g_emit.xa_bb_emit_pair_jmp[_i]=(t); } while(0)
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
                fprintf(stderr, "FATAL drive_value_slot: IR op=%d is a value-producer with no ZLS slot — ir_drive_slot_assign never granted it. "
                        "Emit-time allocation is ERADICATED (TMP-ERADICATE); add the grant in LOWER, never patch it here. op=%d\n", (int)nd->op, (int)nd->op);
        abort();
    }
    fprintf(stderr, "[TE] GOUGE drive_value_slot(op=%d): non-value-producer asked for a slot with no LOWER grant — emit-time allocation is ERADICATED "
                    "(TMP-ERADICATE); add an ir_drive_slot_assign grant for this op\n", nd ? (int)nd->op : -1);
    abort();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_repalt(IR_t **nodes, int n, int i, bb_label_t **lbls, bb_label_t **betas, bb_label_t **ra_y, bb_label_t **ra_t, bb_label_t *node_γ, bb_label_t *node_ω) {
    IR_t *e_root  = nodes[i]->n_operands > 0 ? nodes[i]->operands[0] : NULL;
    IR_t *e_entry = (nodes[i]->n_operands > 1 && nodes[i]->operands[1]) ? nodes[i]->operands[1] : e_root;
    int off = drive_value_slot(nodes[i]);
    int e_sa = e_root ? bb_slot_get(e_root) : -1;
    if (e_sa < 0 && e_root) e_sa = drive_value_slot(e_root);
    bb_label_t *e_lbl = node_ω; bb_label_t *e_beta = ra_t[i];
    if (e_entry) for (int k = 0; k < n; k++) if (nodes[k] == e_entry) { e_lbl = lbls[k]; break; }
    if (e_root && ir_is_generator_kind(e_root->op)) for (int k = 0; k < n; k++) if (nodes[k] == e_root) { e_beta = betas[k]; break; }
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
static int fc_seq_on(const IR_t *nd) { return nd && (x86_port_mode() == ZC_PORT_FORTH || x86_port_mode() == ZC_PORT_HEAP) && fc_seq_active(nd); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int fc_seq_elem_of(IR_t **nodes, int n, int k, int src) {
    int N = (int)(nodes[k]->n_operands / 2), el = 0, sp = -1;
    for (int j = 0; j < N; j++) if (nodes[k]->operands[2 * j] == nodes[src] || nodes[k]->operands[2 * j + 1] == nodes[src]) return j;
    for (int p = 0; p < n; p++) if (nodes[p] == nodes[src]) { sp = p; break; }
    for (int j = 0; j < N; j++) { IR_t *e = nodes[k]->operands[2 * j]; for (int p = 0; p < n; p++) if (nodes[p] == e) { if (p <= sp) el = j; break; } }
    return el;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static bb_label_t * fc_seq_sigma_tgt(IR_t **nodes, int n, int k, int src, bb_label_t **lbls, bb_label_t *dflt) {
    int N = (int)(nodes[k]->n_operands / 2), el = fc_seq_elem_of(nodes, n, k, src);
    if (el >= N - 1) return dflt;
    IR_t *nx = nodes[k]->operands[2 * (el + 1)];
    for (int p = 0; p < n; p++) if (nodes[p] == nx) return lbls[p];
    return dflt;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static bb_label_t * fc_seq_phi_tgt(IR_t **nodes, int n, int k, int src, bb_label_t **betas, bb_label_t *dflt) {
    int el = fc_seq_elem_of(nodes, n, k, src);
    if (el <= 0) return dflt;
    IR_t *pv = nodes[k]->operands[2 * (el - 1) + 1];
    for (int p = 0; p < n; p++) if (nodes[p] == pv) return betas[p];
    return dflt;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void flat_drive_match_alt(IR_t **nodes, int n, int i, bb_label_t **lbls, bb_label_t **betas, bb_label_t **na_s, bb_label_t **na_f, bb_label_t ***fc_sig, bb_label_t *node_γ, bb_label_t *node_ω, bb_label_t *chain_ω) {
    IR_t *nd = nodes[i]; int N = (nd->op == IR_MATCH_ARBNO) ? 1 : (nd->op == IR_DISJUNCTION) ? (int)IR_LIT(nd).ival : nd->n_operands / 2;
    if (3 * N + 2 > XA_BB_EMIT_PAIR_MAX) { fprintf(stderr, "FATAL emit flat_drive_match_alt: node n%d op=%d N=%d needs %d pair slots > XA_BB_EMIT_PAIR_MAX(%d) — raise the constant in emit.h\n", i, (int)nd->op, N, 3 * N + 2, XA_BB_EMIT_PAIR_MAX); abort(); }
    g_emit.xa_bb_emit_pair_n = 0;
    for (int j = 0; j < N; j++) { IR_t *e = nd->operands[2 * j];     bb_label_t *t = node_ω; for (int k = 0; k < n; k++) if (nodes[k] == e) { t = lbls[k];  break; } if (nd->op == IR_DISJUNCTION && e && e->op == IR_FAIL && chain_ω) t = chain_ω;    int _i = g_emit.xa_bb_emit_pair_n++; g_emit.xa_bb_emit_pair_define[_i] = NULL; g_emit.xa_bb_emit_pair_jmp[_i] = t; }
    for (int j = 0; j < N; j++) { IR_t *r = nd->operands[2 * j + 1]; bb_label_t *t = node_ω; for (int k = 0; k < n; k++) if (nodes[k] == r) { t = betas[k]; break; } if (nd->op == IR_MATCH_ARBNO && nd->n_operands > 3 && nd->operands[3] == nd) t = na_f[i];    if (nd->op == IR_DISJUNCTION && r == nd) t = na_f[i];    int _i = g_emit.xa_bb_emit_pair_n++; g_emit.xa_bb_emit_pair_define[_i] = NULL; g_emit.xa_bb_emit_pair_jmp[_i] = t; }
    { int _i = g_emit.xa_bb_emit_pair_n++; g_emit.xa_bb_emit_pair_define[_i] = na_s[i]; g_emit.xa_bb_emit_pair_jmp[_i] = NULL; }
    { int _i = g_emit.xa_bb_emit_pair_n++; g_emit.xa_bb_emit_pair_define[_i] = na_f[i]; g_emit.xa_bb_emit_pair_jmp[_i] = NULL; }
    if (nd->op == IR_MATCH_ARBNO) { IR_t *_t = (nd->n_operands > 2) ? nd->operands[2] : (IR_t *)0; { IR_t *_s0 = (nd->n_operands > 1) ? nd->operands[1] : (IR_t *)0; int _i0 = -1, _i1 = -1; for (int k = 0; k < n; k++) { if (_s0 && nodes[k] == _s0) _i0 = k; if (_t && nodes[k] == _t) _i1 = k; } if (_i0 > _i1) { int _sw = _i0; _i0 = _i1; _i1 = _sw; } if (_i0 >= 0) for (int k = _i1; k >= _i0; k--) if (nodes[k] && nodes[k] != nd && nodes[k]->γ.node == nd) { _t = nodes[k]; break; } }    bb_label_t *_tb = node_ω; for (int k = 0; k < n; k++) if (_t && nodes[k] == _t) { _tb = betas[k]; break; } int _i = g_emit.xa_bb_emit_pair_n++; g_emit.xa_bb_emit_pair_define[_i] = NULL; g_emit.xa_bb_emit_pair_jmp[_i] = _tb; }
    if (nd->op == IR_MATCH_ALTERNATE && fc_sig && fc_sig[i])
        for (int j = 0; j < N; j++) { int _i = g_emit.xa_bb_emit_pair_n++; g_emit.xa_bb_emit_pair_define[_i] = fc_sig[i][j]; g_emit.xa_bb_emit_pair_jmp[_i] = NULL; }
    g_emit.op_off = drive_value_slot(nd); g_emit.op_ival = (int64_t)N;
    if (nd->op == IR_SCAN_SEQUENCE) { for (int j = 0; j < N && j < 32; j++) g_emit.op_parts_ival[j] = zls_off(nd->operands[2 * j + 1]); g_emit.op_parts_n = N; }
    if (nd->op == IR_DISJUNCTION) { for (int j = 0; j < N && j < 32; j++) { IR_t *rj = (2 * N + j < nd->n_operands) ? nd->operands[2 * N + j] : (IR_t *)0; { int _g = 0; while (rj && rj->op == IR_CONJUNCTION && rj->n_operands > 0 && _g++ < 16) rj = rj->operands[0]; }    int rs = rj ? emit_binop_opnd_slot(rj) : -1; g_emit.op_parts_ival[j] = rs; } g_emit.op_parts_n = N; }
    DRIVE_FILL(nd, lbls[i], node_γ, node_ω, betas[i]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void drive_arg_slots_reserve(int n) {
    if (n <= g_emit.op_arg_slot_cap) return;
    int cap = g_emit.op_arg_slot_cap > 0 ? g_emit.op_arg_slot_cap : 16;
    while (cap < n) cap *= 2;
    int *grown = (int *)realloc(g_emit.op_arg_slot, (size_t)cap * sizeof(int));
    if (!grown) { fprintf(stderr, "FATAL emit_drive: arg-slot staging realloc to %d failed\n", cap); abort(); }
    g_emit.op_arg_slot = grown; g_emit.op_arg_slot_cap = cap;
}
#define AB_FNCELL_MAX 1024
static void * g_ab_fn_cells[AB_FNCELL_MAX];
static int    g_ab_fn_cell_n = 0;
static char   g_ab_fn_names[AB_FNCELL_MAX][64];
__attribute__((noreturn)) void rt_ab_undef_fn_stub(void);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static unsigned ab_fn_hash(const char * s) { unsigned h = 2166136261u; for (const unsigned char * p = (const unsigned char *)s; *p; p++) { h ^= *p; h *= 16777619u; } return h; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int ab_hash_on(void) { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_AB_HASH"); v = (e && *e == '0') ? 0 : 1; } return v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
__attribute__((noreturn)) static void ab_slot_full(const char * fname) {
    fprintf(stderr, "FATAL bb_ab_slot_for: cell table full (%d) at '%s' -- raise AB_FNCELL_MAX (R-1 s94: old arm aliased slot 0 SILENTLY, the corruption this abort replaces)\n", AB_FNCELL_MAX, fname);
    abort();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int bb_ab_slot_for(const char * fname) {
    int hashed = ab_hash_on();
    unsigned h = hashed ? (ab_fn_hash(fname) & (AB_FNCELL_MAX - 1)) : 0;
    int cap = hashed ? AB_FNCELL_MAX : (g_ab_fn_cell_n + 1);
    for (int step = 0; step < cap; step++) {
        int i = hashed ? (int)((h + (unsigned)step) & (AB_FNCELL_MAX - 1)) : step;
        int empty = hashed ? (g_ab_fn_names[i][0] == '\0') : (step == g_ab_fn_cell_n);
        if (empty) {
            if (g_ab_fn_cell_n >= AB_FNCELL_MAX) ab_slot_full(fname);
            g_ab_fn_cell_n++;
            snprintf(g_ab_fn_names[i], sizeof g_ab_fn_names[i], "%s", fname);
            g_ab_fn_cells[i] = (void *)(uintptr_t)rt_ab_undef_fn_stub;
            return i;
        }
        if (!strncmp(g_ab_fn_names[i], fname, sizeof g_ab_fn_names[0] - 1)) return i;
    }
    ab_slot_full(fname);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void * bb_ab_cell_addr(const char * fname) { return MEDIUM_BINARY ? (void *)&g_ab_fn_cells[bb_ab_slot_for(fname)] : (void *)0; }
void * bb_ab_fn_cell_ptr(const char * fname) { return (void *)&g_ab_fn_cells[bb_ab_slot_for(fname)]; }
/*⭐ SLOT-KEYED TWIN OF bb_ab_fn_cell_ptr (hq_P s266).  The cell ARRAY is a file-static of fixed extent, so a slot index names the same cell for the life of the process
  and the caller can resolve the NAME once instead of on every call.  rt_call_proc_descr used to rebuild "alpha$<fn>", FNV-hash it and linear-probe this table on EVERY
  SNOBOL4 function call -- 402 Ir/call, 5.1% of the json deserializer.  Nothing here changes what a lookup ANSWERS; it only lets the answer be remembered. */
int    bb_ab_slot_index(const char * fname) { return bb_ab_slot_for(fname); }
void * bb_ab_cell_at(int slot) { return (slot >= 0 && slot < AB_FNCELL_MAX) ? (void *)&g_ab_fn_cells[slot] : (void *)0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void drive_unowned(IR_t *nd) {
    fprintf(stderr, "FATAL emit_drive: IR op=%d has no template in the universal driver. Every op must be handled; the driver never refuses silently. "
                    "Implement op=%d. NOTE: this is also the guard sink inside existing cases — if op=N plainly has a case, the BACKTRACE LINE "
                    "(not this message) names the failing guard.\n",
            nd ? (int)nd->op : -1, nd ? (int)nd->op : -1);
    abort();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void emit_drive(IR_t *nd, bb_label_t *lbl_α, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    if (!nd) { drive_unowned(nd); return; }
    { extern int g_scan_regs_live; g_scan_regs_live = nd->in_scan ? 1 : 0; }
    switch (nd->op) {
    case IR_LIT_STRING: case IR_LIT_INTEGER: case IR_LIT_REAL:
        g_emit.op_off = drive_value_slot(nd); DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    case IR_LIT_CHARSET:
    case IR_LIT_NAME:
        g_emit.op_sval = IR_LIT(nd).sval; g_emit.op_off = drive_value_slot(nd); DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    case IR_KEYWORD_ICON:
    case IR_KEYWORD_ICON_GEN:
    case IR_KEYWORD_SNOBOL4:
        g_emit.op_sval = IR_LIT(nd).sval; g_emit.op_off = drive_value_slot(nd); DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    case IR_KEYWORD_ASSIGN:
    case IR_KEYWORD_ASSIGN_SNOBOL4:
        g_emit.op_sval = IR_LIT(nd).sval; g_emit.op_off = drive_value_slot(nd); DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    case IR_VAR: {
        const char *vn = IR_LIT(nd).sval;
        if (vn && vn[0] == '&') { g_emit.op_sval = vn; g_emit.op_sa = -1; g_emit.op_off = drive_value_slot(nd); }
        else if (vn && is_global(vn) && !graph_has_local(g_emit_cfg, vn)) { g_emit.op_sa = -1; g_emit.op_off = drive_value_slot(nd); g_emit.op_sval = vn; g_emit.op_gva_k = g_gva_active ? gva_index_of(vn) : -1; }
        else if (vn) { int voff = bb_varslot_peek(vn); g_emit.op_sa = voff; g_emit.op_off = (voff != -1) ? drive_value_slot(nd) : -1; }
        else { g_emit.op_sa = -1; g_emit.op_off = -1; }
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_VAR_REF: {
        const char *vn = IR_LIT(nd).sval;
        if (vn && is_global(vn) && !graph_has_local(g_emit_cfg, vn)) { g_emit.op_sa = -1; g_emit.op_off = drive_value_slot(nd); g_emit.op_sval = vn; g_emit.op_gva_k = g_gva_active ? gva_index_of(vn) : -1; }
        else if (vn) { int voff = bb_varslot_peek(vn); g_emit.op_sa = voff; g_emit.op_off = drive_value_slot(nd); }
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
        if (sa < 0 || sb < 0) { drive_unowned(nd); break; }
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
        if (sa < 0) { drive_unowned(nd); break; }
        g_emit.op_sa = sa; g_emit.op_off = drive_value_slot(nd); DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_FIELD_GET: case IR_FIELD_VAR: {
        IR_t * obj = bb_child0(nd);
        int sa = obj ? emit_binop_opnd_slot(obj) : -1;
        if (sa < 0) { drive_unowned(nd); break; }
        g_emit.op_a_slot = sa; g_emit.op_sval = IR_LIT(nd).sval; g_emit.op_off = drive_value_slot(nd);
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
        { int voff = bb_varslot_peek(vn);
          if (voff == -1) { fprintf(stderr, "[TE-4] IR_ASSIGN local '%s' has no LOWER-granted varslot — grant it in ir_drive_slot_assign (scrip_ir.c), never allocate in the emitter\n", vn); abort(); }
          g_emit.op_sb = voff; }
        g_emit.op_off = drive_value_slot(nd); DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_CALL: case IR_CALL_BUILTIN: case IR_CALL_BUILTIN_GEN: case IR_CALL_PROC_STAGED: case IR_PROC_GEN:
    case IR_CALL_BUILTIN_ICON: case IR_CALL_BUILTIN_SNOBOL4: case IR_CALL_BUILTIN_PROLOG: {
        int na = nd->n_operands; drive_arg_slots_reserve(na);
        for (int i = 0; i < na; i++) { IR_t * a = ir_call_arg(nd, i); g_emit.op_arg_slot[i] = nd_slot(a); }
        g_emit.op_arg_slot_n = na; g_emit.op_write_route = bb_call_write_route(nd);
        { extern int bb_scc_handoff_consume(const void *, const char *); g_emit.op_c2 = bb_scc_handoff_consume((const void *)nd, IR_LIT(nd).sval); }
        { IR_t *_gt = nd->γ.node; int _gg = 0; while (_gt && _gt->op == IR_GOTO && _gg++ < 128) _gt = _gt->γ.node; IR_t *_ot = nd->ω.node; int _og = 0; while (_ot && _ot->op == IR_GOTO && _og++ < 128) _ot = _ot->γ.node; g_emit.op_sb = (g_emit_cfg && g_emit_cfg->icn_cells_graph && _gt && _ot && _gt == _ot && (unsigned char)nd->ω.sz[0] == 0xce && (unsigned char)nd->ω.sz[1] == 0xb2) ? 1 : 0; }
        DRIVE_PAIR_RESET(); DRIVE_PAIR_DEF_JMP(lbl_β, lbl_ω); DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_MATCH_BEGIN: {
        IR_t *subj = nd->n_operands > 0 ? nd->operands[0] : (IR_t *)0;
        g_emit.op_sa = subj ? drive_value_slot(subj) : -1;
        { extern int fc_vread_fp(const IR_t *); g_emit.op_subj_cell = (x86_zc_frame() == ZC_FRAME_RSP && (x86_port_mode() == ZC_PORT_FORTH || x86_port_mode() == ZC_PORT_HEAP) && subj && fc_vread_fp(nd) >= 0) ? 1 : 0; }
        g_emit.op_off = drive_value_slot(nd);
        g_emit.op_frame_extra = emit_match_begin_frame_extra(nd);
        DRIVE_PAIR_RESET();
        if (g_emit.lbl_t0_p) { for (int _q = 0; _q < 3; _q++) { int _z = g_emit.xa_bb_emit_pair_n++; g_emit.xa_bb_emit_pair_define[_z] = NULL; g_emit.xa_bb_emit_pair_jmp[_z] = NULL; } int _z = g_emit.xa_bb_emit_pair_n++; g_emit.xa_bb_emit_pair_define[_z] = g_emit.lbl_t0_p; g_emit.xa_bb_emit_pair_jmp[_z] = NULL; g_emit.lbl_t0_p = NULL; g_emit.lbl_t0 = NULL; }
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_MATCH_END: {
        IR_t *hd = nd->n_operands > 0 ? nd->operands[0] : (IR_t *)0;
        g_emit.op_off = hd ? drive_value_slot(hd) : -1;
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
        g_emit.op_sb  = rp ? drive_value_slot(rp) : -1;
        g_emit.op_sa  = sb ? drive_value_slot(sb) : -1;
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
        drive_unowned(nd);
        break;
    }
    case IR_MATCH_ABORT: {
        g_emit.op_off = -1; DRIVE_PAIR_RESET(); DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_MATCH_ALTERNATE: case IR_MATCH_ARBNO: case IR_SCAN_SEQUENCE: case IR_SCAN_ALTERNATE: {
        drive_unowned(nd);
        break;
    }
    case IR_COERCE_STRING: case IR_COERCE_INTEGER: case IR_COERCE_REAL: {
        IR_t * a0 = nd->n_operands > 0 ? nd->operands[0] : (IR_t *)0;
        int sa = a0 ? emit_binop_opnd_slot(a0) : -1;
        if (sa < 0) { drive_unowned(nd); break; }
        g_emit.op_sa = sa; g_emit.op_off = drive_value_slot(nd);
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_COERCE_NUMERIC: case IR_CMP_TEST: case IR_IDENT: case IR_DIFFER: {
        IR_t * a0 = nd->n_operands > 0 ? nd->operands[0] : (IR_t *)0;
        IR_t * a1 = nd->n_operands > 1 ? nd->operands[1] : (nd->op == IR_COERCE_NUMERIC ? a0 : (IR_t *)0);
        int sa = a0 ? emit_binop_opnd_slot(a0) : -1; int sb = a1 ? emit_binop_opnd_slot(a1) : -1;
        if (sa < 0 || sb < 0) { drive_unowned(nd); break; }
        g_emit.op_sa = sa; g_emit.op_sb = sb; g_emit.op_off = drive_value_slot(nd);
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_MATCH_LEN: {
        IR_t * a0 = nd->n_operands > 0 ? nd->operands[0] : (IR_t *)0;
        g_emit.op_sa = a0 ? bb_slot_get(a0) : -1; if (a0 && g_emit.op_sa < 0) { extern int zls_off(const IR_t *); int _zsa = zls_off(a0); if (_zsa >= 0) { g_emit.op_sa = _zsa; g_emit.op_zres = 1; } } g_emit.op_off = -1;
        if (a0 && g_emit.op_sa < 0) { drive_unowned(nd); break; }
        { const char * _sv = (nd->n_operands == 0 && (uintptr_t)(uint64_t)IR_LIT(nd).ival > (uintptr_t)0xFFFFU) ? IR_LIT(nd).sval : (const char *)0; g_emit.op_sval = (_sv && _sv[0] == '*') ? _sv : NULL; }
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
        if (a0 && g_emit.op_sa < 0) { drive_unowned(nd); break; }
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_MATCH_SPAN: {
        IR_t * a0 = nd->n_operands > 0 ? nd->operands[0] : (IR_t *)0;
        g_emit.op_sa = a0 ? bb_slot_get(a0) : -1;
        if (a0 && g_emit.op_sa < 0) { drive_unowned(nd); break; }
        g_emit.x86_scratch_off = drive_value_slot(nd);
        g_emit.op_leaf_frame_off = leaf_frame_slot(nd);
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_MATCH_BREAK: case IR_MATCH_BREAKX: {
        IR_t * a0 = nd->n_operands > 0 ? nd->operands[0] : (IR_t *)0;
        g_emit.op_sa = a0 ? bb_slot_get(a0) : -1;
        if (a0 && g_emit.op_sa < 0) { drive_unowned(nd); break; }
        g_emit.x86_scratch_off = drive_value_slot(nd);
        g_emit.op_leaf_frame_off = leaf_frame_slot(nd);
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_MATCH_TAB: case IR_MATCH_RTAB: {
        IR_t * a0 = nd->n_operands > 0 ? nd->operands[0] : (IR_t *)0;
        g_emit.op_off = -1;
        if (!a0) { g_emit.op_sb = (int)IR_LIT(nd).ival; g_emit.op_sa = -1; }
        else if (a0->op == IR_LIT_INTEGER) { g_emit.op_sb = (int)IR_LIT(a0).ival; g_emit.op_sa = -1; }
        else { int sl = bb_slot_get(a0); if (sl < 0) { drive_unowned(nd); break; } g_emit.op_sa = sl; g_emit.op_sb = 0; }
        g_emit.x86_scratch_off = drive_value_slot(nd);
        g_emit.op_leaf_frame_off = leaf_frame_slot(nd);
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_MATCH_POS: case IR_MATCH_RPOS: {
        IR_t * a0 = nd->n_operands > 0 ? nd->operands[0] : (IR_t *)0;
        g_emit.op_off = -1;
        if (!a0) { g_emit.op_sb = (int)IR_LIT(nd).ival; g_emit.op_sa = -1; }
        else if (a0->op == IR_LIT_INTEGER) { g_emit.op_sb = (int)IR_LIT(a0).ival; g_emit.op_sa = -1; { extern int fc_frameless_fpr_rsp(const IR_t *); if (g_zd_arm && fc_frameless_fpr_rsp(a0)) g_emit.op_wpop += 16; } }
        else { int sl = bb_slot_get(a0); if (sl < 0) { drive_unowned(nd); break; } g_emit.op_sa = sl; g_emit.op_sb = 0; }
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_MATCH_REM: case IR_MATCH_ARB: case IR_MATCH_BAL: {
        g_emit.x86_scratch_off = drive_value_slot(nd);
        g_emit.op_leaf_frame_off = leaf_frame_slot(nd);
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
        if (!bb_child0(nd) || !bb_child1(nd)) { drive_unowned(nd); break; }
        g_emit.op_sa = drive_value_slot(bb_child0(nd)); g_emit.op_sb = drive_value_slot(bb_child1(nd));
        g_emit.op_sc = -1;
        g_emit.op_num_real = (IR_LIT(nd).sval && strcmp(IR_LIT(nd).sval, "ar") == 0) ? 1 : 0;
        g_emit.op_off = drive_value_slot(nd);
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_TO_BY: {
        if (!bb_child0(nd) || !bb_child1(nd) || nd->n_operands < 3 || !nd->operands[2]) { drive_unowned(nd); break; }
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
        if (!vn || nd->n_operands < 1 || !nd->operands[0]) { drive_unowned(nd); break; }
        { int voff = bb_varslot_peek(vn);
          if (voff == -1) { fprintf(stderr, "[TE-4] IR_REV_ASSIGN local '%s' has no LOWER-granted varslot — grant it in ir_drive_slot_assign (scrip_ir.c), never allocate in the emitter\n", vn); abort(); }
          g_emit.op_sb = voff; }
        g_emit.op_off = drive_value_slot(nd);
        g_emit.op_sc  = g_emit.op_off + 16;
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_REV_SWAP: {
        const char * ln = IR_LIT(nd).sval;
        const char * rn = (nd->n_operands > 0 && nd->operands[0]) ? IR_LIT(nd->operands[0]).sval : NULL;
        if (!ln || !rn) { drive_unowned(nd); break; }
        g_emit.op_sval = ln; g_emit.op_name2 = rn;
        g_emit.op_sb = -1; g_emit.op_sa = -1;
        if (ln[0] != '&') { int voff = bb_varslot_peek(ln); if (voff == -1) { fprintf(stderr, "[TE-4] IR_REV_SWAP lhs local '%s' has no LOWER-granted varslot — grant it in ir_drive_slot_assign (scrip_ir.c), never allocate in the emitter\n", ln); abort(); } g_emit.op_sb = voff; }
        if (rn[0] != '&') { int voff = bb_varslot_peek(rn); if (voff == -1) { fprintf(stderr, "[TE-4] IR_REV_SWAP rhs local '%s' has no LOWER-granted varslot — grant it in ir_drive_slot_assign (scrip_ir.c), never allocate in the emitter\n", rn); abort(); } g_emit.op_sa = voff; }
        g_emit.op_off = drive_value_slot(nd);
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_REV_ASSIGN_VAR: {
        IR_t * v = nd->n_operands > 0 ? nd->operands[0] : NULL;
        IR_t * r = nd->n_operands > 1 ? nd->operands[1] : NULL;
        int sa = v ? drive_value_slot(v) : -1; int sb = r ? drive_value_slot(r) : -1;
        if (sa < 0 || sb < 0) { drive_unowned(nd); break; }
        g_emit.op_sa  = sb;
        g_emit.op_off = drive_value_slot(nd);
        g_emit.op_sc  = g_emit.op_off + 16;
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_GOTO:
        DRIVE_PAIR_RESET(); DRIVE_PAIR_JMP(lbl_γ); DRIVE_PAIR_DEF_JMP(lbl_β, lbl_ω); DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    case IR_BOUND: {
        g_emit.op_sb = 1; g_emit.op_off = zls_off(nd); g_emit.op_fc_bytes = 0;
        if (g_emit.op_off < 0) { drive_unowned(nd); break; }
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_UNMARK: {
        IR_t * _mk = nd->n_operands > 0 ? nd->operands[0] : (IR_t *)0;
        g_emit.op_sb = 0; g_emit.op_off = _mk ? zls_off(_mk) : -1; g_emit.op_fc_bytes = 0;
        if (g_emit.op_off < 0) { drive_unowned(nd); break; }
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_CONJUNCTION:
        if (nd->n_operands > 0 && nd->operands[0] && bb_slot_get(nd) < 0) { int voff = bb_slot_get(nd->operands[0]); if (voff >= 0) bb_slot_register(nd, voff); }
        { int dst = nd_slot(nd); if (dst >= 0 && nd->n_operands > 0 && nd->operands[0]) {
            int src = bb_slot_get(nd->operands[0]);
            if (src >= 0 && dst >= 0 && src != dst) {
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
        { extern void bb_scc_handoff_pending_set(const void *, const char *); extern void bb_scc_handoff_pending_clear(void);
          if (g_emit.op_ival == 0 && g_emit.op_sval && nd->γ.node) bb_scc_handoff_pending_set((const void *)nd->γ.node, g_emit.op_sval); else bb_scc_handoff_pending_clear(); }
        g_emit.op_c2 = 0;
        int na = nd->n_operands; drive_arg_slots_reserve(na);
        for (int i = 0; i < na; i++) { IR_t * a = nd->operands[i]; int s = a ? bb_slot_get(a) : -1; if (s < 0 && a) s = zls_off(a); g_emit.op_arg_slot[i] = s; }
        g_emit.op_arg_slot_n = na;
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
        if (nd->n_operands != 0) { drive_unowned(nd); break; }
        DRIVE_PAIR_RESET(); DRIVE_PAIR_JMP(lbl_γ); DRIVE_PAIR_DEF_JMP(lbl_β, lbl_ω); DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    case IR_STATEMENT_BEGIN:
    case IR_STATEMENT_END:
    case IR_STATEMENT:
        g_emit.op_fc_bytes = 0;
        DRIVE_PAIR_RESET(); DRIVE_PAIR_JMP(lbl_γ); DRIVE_PAIR_DEF_JMP(lbl_β, lbl_ω); DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    case IR_SUSPEND: {
        IR_t * ev = bb_child0(nd); int sa = ev ? bb_slot_get(ev) : -1;
        if (sa < 0 && ev) sa = nd_slot(ev);
        if (sa < 0) { drive_unowned(nd); break; }
        g_emit.op_sa = sa; g_emit.lbl_t0 = g_suspend_dobody_beta ? g_suspend_dobody_beta->name : NULL; g_emit.lbl_t0_p = g_suspend_dobody_beta;
        g_emit.lbl_t1_p = lbl_β; g_emit.lbl_t1 = lbl_β ? lbl_β->name : NULL;
        g_emit.op_sb    = g_suspend_resume_slot;
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_SUBSCRIPT: {
        if (nd->n_operands == 2) {
            IR_t * vb = nd->operands[0]; IR_t * vi = nd->operands[1];
            int va = vb ? drive_value_slot(vb) : -1; int vs = vi ? drive_value_slot(vi) : -1;
            if (va < 0 || vs < 0) { drive_unowned(nd); break; }
            g_emit.op_a_slot = va; g_emit.op_sa = vs; g_emit.op_off = drive_value_slot(nd);
            DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
        }
        IR_t * base = nd->n_operands > 0 ? nd->operands[0] : NULL;
        IR_t * i1   = nd->n_operands > 1 ? nd->operands[1] : NULL;
        IR_t * i2   = nd->n_operands > 2 ? nd->operands[2] : NULL;
        int sa = base ? drive_value_slot(base) : -1;
        int sb = i1   ? drive_value_slot(i1)   : -1;
        int sc = i2   ? drive_value_slot(i2)   : -1;
        if (sa < 0 || sb < 0 || sc < 0) { drive_unowned(nd); break; }
        g_emit.op_a_slot = sa; g_emit.op_sa = sb; g_emit.op_sb = sc;
        g_emit.op_off = drive_value_slot(nd);
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_RANDOM: {
        IR_t * v = nd->n_operands > 0 ? nd->operands[0] : NULL;
        int sa = v ? drive_value_slot(v) : -1;
        if (sa < 0) { drive_unowned(nd); break; }
        g_emit.op_a_slot = sa; g_emit.op_off = drive_value_slot(nd);
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_DEREF: {
        IR_t * v = nd->n_operands > 0 ? nd->operands[0] : NULL;
        int sa = v ? drive_value_slot(v) : -1;
        if (sa < 0) { drive_unowned(nd); break; }
        g_emit.op_a_slot = sa; g_emit.op_off = drive_value_slot(nd);
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_ASSIGN_VAR: {
        IR_t * v = nd->n_operands > 0 ? nd->operands[0] : NULL;
        IR_t * r = nd->n_operands > 1 ? nd->operands[1] : NULL;
        int sa = v ? drive_value_slot(v) : -1; int sb = r ? drive_value_slot(r) : -1;
        if (sa < 0 || sb < 0) { drive_unowned(nd); break; }
        g_emit.op_a_slot = sa; g_emit.op_sa = sb; g_emit.op_off = drive_value_slot(nd);
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_SWAP: {
        IR_t * xv = nd->n_operands > 0 ? nd->operands[0] : NULL;
        IR_t * yv = nd->n_operands > 1 ? nd->operands[1] : NULL;
        int sa = xv ? bb_varslot_peek(IR_LIT(xv).sval) : -1;
        int sb = yv ? bb_varslot_peek(IR_LIT(yv).sval) : -1;
        if (sa < 0 || sb < 0) { drive_unowned(nd); break; }
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
        if (sc < 0 || na2 < 0) { drive_unowned(nd); break; }
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
        if (sa < 0 || sb < 0) { drive_unowned(nd); break; }
        g_emit.op_a_slot = sa; g_emit.op_sa = sb; g_emit.op_off = drive_value_slot(nd);
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_LIMIT: {
        IR_t * gen = nd->n_operands > 0 ? nd->operands[0] : NULL;
        IR_t * cnt = nd->n_operands > 1 ? nd->operands[1] : NULL;
        int sa = gen ? drive_value_slot(gen) : -1;
        int sc = cnt ? drive_value_slot(cnt) : -1;
        if (sa < 0 || sc < 0) { drive_unowned(nd); break; }
        g_emit.op_sa = sa; g_emit.op_sc = sc;
        g_emit.lbl_t0   = g_limit_gen_beta ? g_limit_gen_beta->name : (lbl_β ? lbl_β->name : NULL);
        g_emit.lbl_t0_p = g_limit_gen_beta ? g_limit_gen_beta : lbl_β;
        g_emit.op_off = drive_value_slot(nd);
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_CREATE: {
        g_emit.op_off = drive_value_slot(nd);
        g_emit.lbl_t0   = g_create_body_entry ? g_create_body_entry->name : NULL;
        g_emit.lbl_t0_p = g_create_body_entry;
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_ACTIVATE: {
        IR_t * ce = nd->n_operands > 0 ? nd->operands[0] : NULL;
        IR_t * xv = nd->n_operands > 1 ? nd->operands[1] : NULL;
        int sa = ce ? bb_slot_get(ce) : -1;
        if (sa < 0) { drive_unowned(nd); break; }
        g_emit.op_sa = sa;
        g_emit.op_sb = xv ? bb_slot_get(xv) : -1;
        g_emit.op_off = drive_value_slot(nd);
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_CORET: {
        IR_t * pv = nd->n_operands > 0 ? nd->operands[0] : NULL;
        int sa = pv ? bb_slot_get(pv) : -1;
        if (sa < 0) { drive_unowned(nd); break; }
        g_emit.op_sa = sa;
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_COFAIL: {
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_MOVE_LABEL: {
        IR_t * ig = nd->n_operands > 1 ? nd->operands[1] : NULL;
        IR_t * av = nd->n_operands > 2 ? nd->operands[2] : NULL;
        int off = ig ? drive_value_slot(ig) : -1;
        g_emit.op_off = off;
        int avs = av ? bb_slot_get(av) : -1;
        if (avs < 0 && av) avs = nd_slot(av);
        g_emit.op_sa = avs;
        g_emit.lbl_t0 = g_move_label_tgt ? g_move_label_tgt->name : NULL;
        g_emit.lbl_t0_p = g_move_label_tgt;
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_DISJUNCTION: {
        if (nd->n_operands > 0) { drive_unowned(nd); break; }
        int off = drive_value_slot(nd);
        g_emit.op_off = off;
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_ITERATE: {
        IR_t * obj = nd->n_operands > 0 ? nd->operands[0] : NULL;
        int sa = obj ? bb_slot_get(obj) : -1;
        if (sa < 0 && obj) sa = nd_slot(obj);
        if (sa < 0) { drive_unowned(nd); break; }
        g_emit.op_sa = sa;
        g_emit.op_off = drive_value_slot(nd);
        g_emit.op_sb  = g_emit.op_off + 16;
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_INITIAL: {
        g_emit.op_off = nd_slot(nd);
        if (g_emit.op_off < 0) { drive_unowned(nd); break; }
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_SCAN_ENTER: {
        IR_t * subj = nd->n_operands > 0 ? nd->operands[0] : NULL;
        int sa = subj ? bb_slot_get(subj) : -1;
        if (sa < 0 && subj) sa = nd_slot(subj);
        if (sa < 0) { drive_unowned(nd); break; }
        g_emit.op_sa = sa; g_emit.op_sb = 1;
        g_emit.op_off = drive_value_slot(nd);
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_SCAN: {
        g_emit.op_sb  = 0;
        IR_t * enter_nd = nd->n_operands > 0 ? nd->operands[0] : NULL;
        g_emit.op_off = nd_slot(enter_nd);
        if (g_emit.op_off < 0) { drive_unowned(nd); break; }
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_SCAN_TAB: {
        IR_t * a0 = nd->n_operands > 0 ? nd->operands[0] : NULL;
        g_emit.op_off = drive_value_slot(nd);
        if (!a0) { drive_unowned(nd); break; }
        if (a0->op == IR_LIT_INTEGER) { g_emit.op_sb = (int)IR_LIT(a0).ival; g_emit.op_sa = -1; }
        else { int sl = bb_slot_get(a0); if (sl < 0) sl = nd_slot(a0); if (sl < 0) { drive_unowned(nd); break; } g_emit.op_sa = sl; g_emit.op_sb = 0; }
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_SCAN_MOVE: {
        IR_t * a0 = nd->n_operands > 0 ? nd->operands[0] : NULL;
        g_emit.op_off = drive_value_slot(nd);
        if (!a0) { drive_unowned(nd); break; }
        if (a0->op == IR_LIT_INTEGER) { g_emit.op_sb = (int)IR_LIT(a0).ival; g_emit.op_sa = -1; }
        else { int sl = bb_slot_get(a0); if (sl < 0) { drive_unowned(nd); break; } g_emit.op_sa = sl; g_emit.op_sb = 0; }
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_SCAN_POS: {
        IR_t * a0 = nd->n_operands > 0 ? nd->operands[0] : NULL;
        g_emit.op_off = drive_value_slot(nd);
        if (!a0) { drive_unowned(nd); break; }
        if (a0->op == IR_LIT_INTEGER) { g_emit.op_sb = (int)IR_LIT(a0).ival; g_emit.op_sa = -1; }
        else { int sl = bb_slot_get(a0); if (sl < 0) { drive_unowned(nd); break; } g_emit.op_sa = sl; g_emit.op_sb = 0; }
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_SCAN_UPTO: {
        IR_t * a0 = nd->n_operands > 0 ? nd->operands[0] : NULL;
        g_emit.op_off = drive_value_slot(nd);
        if (a0 && (a0->op == IR_LIT_STRING || a0->op == IR_LIT_CHARSET) && IR_LIT(a0).sval) {
            g_emit.op_name1 = IR_LIT(a0).sval; g_emit.op_sa = -1;
        } else if (a0) {
            int sl = bb_slot_get(a0); if (sl < 0) { drive_unowned(nd); break; } g_emit.op_sa = sl; g_emit.op_name1 = NULL;
        } else { drive_unowned(nd); break; }
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_SCAN_ANY: {
        IR_t * a0 = nd->n_operands > 0 ? nd->operands[0] : NULL;
        g_emit.op_off = drive_value_slot(nd);
        if (a0 && (a0->op == IR_LIT_STRING || a0->op == IR_LIT_CHARSET) && IR_LIT(a0).sval) {
            g_emit.op_name1 = IR_LIT(a0).sval; g_emit.op_sa = -1;
        } else if (a0) {
            int sl = bb_slot_get(a0); if (sl < 0) { drive_unowned(nd); break; } g_emit.op_sa = sl; g_emit.op_name1 = NULL;
        } else { drive_unowned(nd); break; }
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
        if (a0 && a0->op == IR_LIT_STRING && IR_LIT(a0).sval) {
            g_emit.op_name1 = IR_LIT(a0).sval; g_emit.op_sa = -1;
        } else if (a0) {
            int sl = bb_slot_get(a0); if (sl < 0) { drive_unowned(nd); break; } g_emit.op_sa = sl; g_emit.op_name1 = NULL;
        } else { drive_unowned(nd); break; }
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_SCAN_MATCH: {
        IR_t * a0 = nd->n_operands > 0 ? nd->operands[0] : NULL;
        g_emit.op_off = drive_value_slot(nd);
        if (a0 && a0->op == IR_LIT_STRING && IR_LIT(a0).sval) {
            g_emit.op_name1 = IR_LIT(a0).sval; g_emit.op_sa = -1;
        } else if (a0) {
            int sl = bb_slot_get(a0); if (sl < 0) { drive_unowned(nd); break; } g_emit.op_sa = sl; g_emit.op_name1 = NULL;
        } else { drive_unowned(nd); break; }
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_SCAN_BAL: {
        IR_t * a0 = nd->n_operands > 0 ? nd->operands[0] : NULL;
        g_emit.op_off = drive_value_slot(nd);
        if (a0 && (a0->op == IR_LIT_STRING || a0->op == IR_LIT_CHARSET) && IR_LIT(a0).sval) {
            g_emit.op_name1 = IR_LIT(a0).sval; g_emit.op_sa = -1;
        } else if (a0) {
            int sl = bb_slot_get(a0); if (sl < 0) { drive_unowned(nd); break; } g_emit.op_sa = sl; g_emit.op_name1 = NULL;
        } else { drive_unowned(nd); break; }
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_REPALT: {
        drive_unowned(nd); break;
    }
    case IR_FAIL:
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    case IR_RETURN:
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    case IR_GLIT:
        g_emit.op_name1 = IR_LIT(nd).sval; DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    case IR_GCC:
        g_emit.op_name1 = IR_LIT(nd).sval; DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    case IR_GALT:
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    default:
        drive_unowned(nd); break;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void emit_zeta_selfload(void) { int m = x86_port_mode(); if (m == ZC_PORT_INSTRUMENTED) { if (g_is_text) { char s[64]; snprintf(s, sizeof s, " test %s, %s\n jnz 1f\n ud2\n1:\n", x86_zr(), x86_zr()); emit_text_n(s, strlen(s)); } else { int z = x86_zr_num(), lo = z & 7; ef_b3((uint8_t)(0x48 | (z >= 8 ? 0x05 : 0x00)), 0x85, (uint8_t)(0xC0 | (lo << 3) | lo)); ef_b2(0x75, 0x02); ef_b2(0x0F, 0x0B); } } }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int flat_trivial_beta(const IR_t *nd) {
    switch (nd->op) {
    case IR_MATCH_POS: case IR_MATCH_RPOS: case IR_MATCH_ABORT: return 1;
    case IR_MATCH_ASSIGN_SAVE: { long _k = 0; return fc_geom(nd, &_k) && _k > 0; }
    default: return 0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int flat_unwind_beta(const IR_t *nd) {
    switch (nd->op) {
    case IR_VAR: case IR_LIT_INTEGER: case IR_LIT_STRING: case IR_BINOP: case IR_UNOP: case IR_ASSIGN: case IR_ASSIGN_VAR:
    case IR_CMP_TEST: case IR_COERCE_NUMERIC: case IR_COERCE_STRING: case IR_COERCE_INTEGER: case IR_COERCE_REAL:
    case IR_IDENT: case IR_DIFFER:
    case IR_DEREF: case IR_SUBSCRIPT: case IR_FIELD_VAR: return 1;
    case IR_MATCH_ALTERNATE: return 1;
    case IR_MATCH_ARBNO: return 1;
    default: return flat_trivial_beta(nd);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * zd_chase(IR_t * t) { int g = 0; while (t && t->op == IR_GOTO && g++ < 128) t = t->γ.node; return t; }
static int port_sz_beta(const char * sz) { return sz[0] == (char)0xce && (unsigned char)sz[1] == 0xb2; }
static int beta_is_stmt_land(const IR_t * tgt) { return tgt && tgt->op == IR_STATEMENT_BEGIN; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int zd_wl_kind(IR_t * nd) {
    int op = (int)nd->op;
    extern int is_global(const char *);
    if (g_emit.zframe_graph && !(g_emit_cfg && (g_emit_cfg->icn_cells_graph || g_emit_cfg->pl_cells_graph))) {
        int op = (int)nd->op;
        if (op == IR_VAR || op == IR_ASSIGN) { extern int is_global(const char *); const char *vn = IR_LIT(nd).sval; if (vn && graph_has_local(g_emit_cfg, vn)) goto zframe_local_admitted; }
        return 0;
    }
    if (!(g_emit_cfg && (g_emit_cfg->icn_cells_graph || g_emit_cfg->pl_cells_graph))) return 1;
    zframe_local_admitted:;
    if (op == IR_LIT_INTEGER || op == IR_LIT_STRING || op == IR_LIT_REAL || op == IR_LIT_CHARSET || op == IR_LIT_NAME) return 1;
    if (op == IR_CALL_PROC_STAGED && !(g_emit_cfg && g_emit_cfg->pl_cells_graph)) { static int _zk2cps = -1; if (_zk2cps < 0) { const char * e = getenv("SCRIP_ZD_ICN_CPS"); _zk2cps = (e && *e == '0') ? 0 : 1; } return (_zk2cps && g_emit_cfg && g_emit_cfg->icn_cells_graph) ? 1 : 0; }
    if (op == IR_UNOP) { int v = (int)IR_LIT(nd).ival; return (v == TT_MNS || v == TT_PLS || v == TT_SIZE || v == TT_CSET_COMPL) ? 1 : 0; }
    if (op == IR_BINOP) { long long o = (long long)IR_LIT(nd).ival; return (o == BINOP_ADD || o == BINOP_SUB || o == BINOP_MUL || o == BINOP_DIV || o == BINOP_MOD || o == BINOP_POW || o == BINOP_CUNION || o == BINOP_CDIFF || o == BINOP_CINTER || binop_is_concat((long)o)) ? 1 : 0; }
    if (op == IR_VAR || op == IR_ASSIGN) { const char * vn = IR_LIT(nd).sval; int _icn_cells = g_emit_cfg && g_emit_cfg->icn_cells_graph; return (vn && ((is_global(vn) && !graph_has_local(g_emit_cfg, vn) && !_icn_cells) || (graph_has_local(g_emit_cfg, vn) && x86_fb_pinned()) || (graph_has_local(g_emit_cfg, vn) && _icn_cells && x86_fb_pinned()))) ? 1 : 0; }
    if (op == IR_COERCE_NUMERIC || op == IR_CMP_TEST) return 1;
    if (op == IR_GOTO_DEFERRED) return 1;
    if (op == IR_GOTO) return 1;
    if (op == IR_STATEMENT) return 1;
    if (op == IR_DEFINE && !ir_define_sr_citizen(nd)) return 1;
    if (op == IR_DEFINE) { static int _sr = -1; if (_sr < 0) { const char * e = getenv("SCRIP_ZD_SR"); _sr = (e && *e == '0') ? 0 : 1; } return (_sr && zd_sr_role(nd) != 0) ? 1 : 0; }
    if (op == IR_STATEMENT_BEGIN || op == IR_STATEMENT_END) return 1;
    if (op == IR_KEYWORD_ASSIGN_SNOBOL4) return 1;
    if (op == IR_KEYWORD_SNOBOL4) return 1;
    if (op == IR_KEYWORD_ICON) { static int _icnkw = -1; if (_icnkw < 0) { const char * e = getenv("SCRIP_ZD_ICN_KW"); _icnkw = (e && *e == '0') ? 0 : 1; } if (!(_icnkw && g_emit_cfg && g_emit_cfg->icn_cells_graph)) return 0; { const char *_kw = IR_LIT(nd).sval; if (!_kw) return 0; const char *_k = (_kw[0] == '&') ? _kw + 1 : _kw; return (!strcmp(_k, "null") || !strcmp(_k, "pos") || !strcmp(_k, "subject")) ? 1 : 0; } }
    if (op == IR_COERCE_STRING || op == IR_COERCE_INTEGER) return 1;
    if (op == IR_DEREF || op == IR_ASSIGN_VAR) return 1;
    if (op == IR_FIELD_VAR) return 1;
    if (op == IR_SUBSCRIPT) return (nd->n_operands == 2) ? 1 : 0;
    if (op == IR_CALL) { extern int rt_proc_is_registered(const char *); extern int rt_proc_is_generator(const char *); const char * fn = IR_LIT(nd).sval; static int _zp = -1; if (_zp < 0) { const char *e = getenv("SCRIP_ZD_PROC"); _zp = (e && *e == '0') ? 0 : 1; } if (fn && rt_proc_is_registered(fn)) { if (!_zp) return 0; return (rt_proc_is_generator(fn)) ? 0 : 1; } return 1; }
    if (op == IR_CALL_BUILTIN_ICON) { extern int icn_builtin_is_generator(const char *); extern int icn_builtin_is_known(const char *); const char * fn = IR_LIT(nd).sval; if (fn && icn_builtin_is_generator(fn)) return 0;  if (fn && icn_builtin_is_known(fn)) return 1; return 1; }
    if (op == IR_MATCH_BEGIN || op == IR_MATCH_END || op == IR_MATCH_REPLACE) return 1;
    { static int _za = -1; if (_za < 0) { const char * e = getenv("SCRIP_ZD_ALT"); _za = (e && *e == '0') ? 0 : 1; }
      if (_za && op == IR_MATCH_ALTERNATE) return 1; }
    { static int _zdf2 = -1; if (_zdf2 < 0) { const char *_e = getenv("SCRIP_ZD_FENCE1"); _zdf2 = (_e && *_e == '0') ? 0 : 1; }
      if (_zdf2 && (op == IR_MATCH_FENCE1 || op == IR_MATCH_FENCE0)) return 1; }
    if (op == IR_MATCH_DEFER && nd->pat_static) return 1;
    if (!(g_emit_cfg && (g_emit_cfg->icn_cells_graph || g_emit_cfg->pl_cells_graph))) return 1;
    if (op == IR_MATCH_LIT) return 1;
    if (op == IR_MATCH_LEN)   return 1;
    if (op == IR_MATCH_ANY)   return 1;
    if (op == IR_MATCH_NOTANY) return 1;
    if (op == IR_MATCH_POS)   return 1;
    if (op == IR_MATCH_RPOS)  return 1;
    if (op == IR_MATCH_TAB)   return 1;
    if (op == IR_MATCH_RTAB)  return 1;
    if (op == IR_MATCH_REM)   return 1;
    { static int _zb = -1; if (_zb < 0) { const char * _e = getenv("SCRIP_ZD_BREAK"); _zb = (_e && *_e == '0') ? 0 : 1; } if (_zb && (op == IR_MATCH_BREAK || op == IR_MATCH_BREAKX)) return 1; }
    if (op == IR_MATCH_SPAN)  return 1;
    { static int _zc = -1; if (_zc < 0) { const char * _e = getenv("SCRIP_ZD_CAP"); _zc = (_e && *_e == '0') ? 0 : 1; }
      if (_zc && (op == IR_MATCH_ASSIGN_SAVE || op == IR_MATCH_ASSIGN_COND || op == IR_MATCH_ASSIGN_IMM)) return 1;
      if (_zc && op == IR_MATCH_VALUE) return 1; }
    if (op == IR_TO) return 1;
    if (op == IR_CALL_BUILTIN_PROLOG) { static int _plcbp = -1; if (_plcbp < 0) { const char * e = getenv("SCRIP_ZD_PL_CBP"); _plcbp = (e && *e == '0') ? 0 : 1; } return (_plcbp && g_emit_cfg && g_emit_cfg->pl_cells_graph) ? 1 : 0; }
    if (op == IR_VAR_REF) { static int _icnvr = -1; if (_icnvr < 0) { const char * e = getenv("SCRIP_ZD_ICN_VR"); _icnvr = (e && *e == '0') ? 0 : 1; } if (_icnvr && g_emit_cfg && g_emit_cfg->icn_cells_graph && x86_fb_pinned()) { const char * _vn = IR_LIT(nd).sval; extern int is_global(const char *); if (_vn && !is_global(_vn)) return 1; }    static int _plvr = -1; if (_plvr < 0) { const char * e = getenv("SCRIP_ZD_PL_VR"); _plvr = (e && *e == '0') ? 0 : 1; } return (_plvr && g_emit_cfg && g_emit_cfg->pl_cells_graph && emit_rec_pin() && emit_pl_gamma_retain()) ? 1 : 0; }
    if (op == IR_CALL_PROC_STAGED) { static int _plcps = -1; if (_plcps < 0) { const char * e = getenv("SCRIP_ZD_PL_CPS"); _plcps = (e && *e == '0') ? 0 : 1; } return (_plcps && g_emit_cfg && g_emit_cfg->pl_cells_graph) ? 1 : 0; }
    if (op == IR_SUSPEND) { static int _plsusp = -1; if (_plsusp < 0) { const char * e = getenv("SCRIP_ZD_PL_SUSP"); _plsusp = (e && *e == '0') ? 0 : 1; } return (_plsusp && g_emit_cfg && g_emit_cfg->pl_cells_graph) ? 1 : 0; }
    if (op == IR_VAR) { static int _plvar = -1; if (_plvar < 0) { const char * e = getenv("SCRIP_ZD_PL_VAR"); _plvar = (e && *e == '0') ? 0 : 1; } return (_plvar && g_emit_cfg && g_emit_cfg->pl_cells_graph) ? 1 : 0; }
    if (op == IR_DISJUNCTION) { static int _pldj = -1; if (_pldj < 0) { const char * e = getenv("SCRIP_ZD_PL_DJ"); _pldj = (e && *e == '0') ? 0 : 1; } return (_pldj && g_emit_cfg && g_emit_cfg->pl_cells_graph && nd->n_operands == 0) ? 1 : 0; }
    if (op == IR_CUT) { static int _plcut = -1; if (_plcut < 0) { const char * e = getenv("SCRIP_ZD_PL_CUT"); _plcut = (e && *e == '0') ? 0 : 1; } return (_plcut && g_emit_cfg && g_emit_cfg->pl_cells_graph) ? 1 : 0; }
    if (op == IR_MOVE_LABEL) { static int _plml = -1; if (_plml < 0) { const char * e = getenv("SCRIP_ZD_PL_ML"); _plml = (e && *e == '0') ? 0 : 1; } return (_plml && g_emit_cfg && g_emit_cfg->pl_cells_graph) ? 1 : 0; }
    if (op == IR_MAKE_LIST) { static int _icnml = -1; if (_icnml < 0) { const char * e = getenv("SCRIP_ZD_ICN_ML"); _icnml = (e && *e == '0') ? 0 : 1; } return (_icnml && g_emit_cfg && g_emit_cfg->icn_cells_graph && x86_fb_pinned()) ? 1 : 0; }
    if (op == IR_RETURN) { static int _zk4r = -1; if (_zk4r < 0) { const char * e = getenv("SCRIP_ZD_RETURN"); _zk4r = (e && *e == '0') ? 0 : 1; } return (_zk4r && g_emit_cfg && g_emit_cfg->icn_cells_graph) ? 1 : 0; }
    if (op == IR_CONJUNCTION) { static int _zk2c = -1; if (_zk2c < 0) { const char * e = getenv("SCRIP_ZD_CONJ"); _zk2c = (e && *e == '0') ? 0 : 1; } return (_zk2c && g_emit_cfg && g_emit_cfg->icn_cells_graph) ? 1 : 0; }
    if (op == IR_BINOP_TEST) { static int _zk2r = -1; if (_zk2r < 0) { const char * e = getenv("SCRIP_ZD_RELOP"); _zk2r = (e && *e == '0') ? 0 : 1; } return (_zk2r && g_emit_cfg && g_emit_cfg->icn_cells_graph) ? 1 : 0; }
    if (op == IR_BOUND || op == IR_UNMARK) { static int _zk3 = -1; if (_zk3 < 0) { const char * l = getenv("SCRIP_ICN_LEGACY"); if (l && *l == '1') { const char * e = getenv("SCRIP_ICN_CELLS"); _zk3 = (e && *e == '1') ? 1 : 0; } else _zk3 = 1; } return (_zk3 && g_emit_cfg && g_emit_cfg->icn_cells_graph) ? 1 : 0; }
    if (op == IR_LIT_INTEGER || op == IR_LIT_STRING || op == IR_LIT_REAL || op == IR_LIT_CHARSET || op == IR_LIT_NAME) return 1;
    { static int _tot = -1; if (_tot < 0) { const char * e = getenv("SCRIP_ZD_TOTAL"); _tot = (e && *e == '1') ? 1 : 0; }
      if (_tot) return (ir_is_matcher(op)) ? 0 : 1; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int zd_nops(IR_t * nd) { int op = (int)nd->op;
    if (op == IR_CALL_PROC_STAGED && g_emit_cfg && g_emit_cfg->pl_cells_graph) return 0;
    if (op == IR_SUSPEND && g_emit_cfg && g_emit_cfg->pl_cells_graph) return 0;
    if (op == IR_KEYWORD_ASSIGN_SNOBOL4) return 1;
    return (op == IR_UNOP || op == IR_ASSIGN || op == IR_DEREF || op == IR_COERCE_STRING || op == IR_COERCE_INTEGER || op == IR_FIELD_VAR || op == IR_MATCH_BEGIN || op == IR_RETURN) ? 1 :    (op == IR_BINOP || op == IR_COERCE_NUMERIC || op == IR_CMP_TEST || op == IR_IDENT || op == IR_DIFFER || op == IR_SUBSCRIPT || op == IR_ASSIGN_VAR || op == IR_BINOP_TEST) ? 2 :    (op == IR_MATCH_REPLACE) ? 2 : (op == IR_MAKE_LIST || op == IR_CALL || op == IR_CALL_BUILTIN_ICON || op == IR_CALL_BUILTIN_PROLOG || op == IR_CALL_PROC_STAGED || op == IR_SUSPEND || op == IR_TO || op == IR_MATCH_LEN || op == IR_MATCH_ANY || op == IR_MATCH_NOTANY || op == IR_MATCH_POS || op == IR_MATCH_RPOS || op == IR_MATCH_TAB || op == IR_MATCH_RTAB || op == IR_MATCH_SPAN || op == IR_MATCH_BREAK || op == IR_MATCH_BREAKX || op == IR_MATCH_ASSIGN_COND || op == IR_MATCH_ASSIGN_IMM || op == IR_MATCH_VALUE) ? (int)nd->n_operands : 0; }
static int emit_graph_has_deep_arrival(IR_graph_t *g);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int emit_match_begin_stfh_k_raw(void) {
    { static int _oscap = -1; if (_oscap < 0) { const char * e = getenv("SCRIP_OS_CAP"); _oscap = (e && *e == '0') ? 0 : 1; } if (!_oscap) return 0; }
    if (!g_emit_cfg) return 0;
    if (!emit_graph_has_deep_arrival(g_emit_cfg)) return 0;
    for (int _i = 0; _i < g_emit_cfg->n; _i++) { IR_t * _nd = g_emit_cfg->all[_i]; if (!_nd) continue; if (_nd->op == IR_MATCH_REPLACE || (_nd->op == IR_MATCH_FENCE1 || _nd->op == IR_MATCH_FENCE0) || _nd->op == IR_MATCH_ABORT || _nd->op == IR_MATCH_ARBNO) return 0; }
    return 64;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int emit_match_rbp(void) {
    static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_MATCH_RBP"); v = (e && *e == '0') ? 0 : 1; }
    return v;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int emit_defer_rbp(void) {
    static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_DEFER_RBP"); v = (e && *e == '0') ? 0 : 1; }
    return v;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int emit_defer_carve_rbp(void) {
    static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_DEFER_CARVE_RBP"); v = (e && *e == '1') ? 1 : 0; }
    return v;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int emit_match_owns_startd(void) { return emit_match_begin_stfh_k_raw() > 0; }
int emit_match_begin_stfh_k(void) { return emit_match_rbp() ? 0 : emit_match_begin_stfh_k_raw(); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int emit_arbno_rbp(void) {
    (void)getenv("SCRIP_ARBNO_RBP");
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int emit_arbno_rbp_unwind(void) { return 0; }
int sn4_arbno_reentry(void);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int arbno_reentry_hazard(const IR_t * nd) {
    int blob_frame_scope(void);
    if (!sn4_arbno_reentry() || !nd || nd->op != IR_MATCH_ARBNO || nd->n_operands < 3 || !g_emit_cfg || !blob_frame_scope()) return 0;
    int s0 = -1, s1 = -1; for (int j = 0; j < g_emit_cfg->n; j++) { if (g_emit_cfg->all[j] == nd->operands[1]) s0 = j; if (g_emit_cfg->all[j] == nd->operands[2]) s1 = j; }
    if (s0 > s1) { int t = s0; s0 = s1; s1 = t; }
    for (int j = 0; j < g_emit_cfg->n; j++) { IR_t * m = g_emit_cfg->all[j]; if (!m || m == nd || (s0 >= 0 && j >= s0 && j <= s1)) continue;
        if (m->op == IR_MATCH_DEFER) return 1; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int sn4_arbno_reentry(void) { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_ARBNO_REENTRY"); v = (e && *e == '1') ? 1 : 0; } return v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int arbno_frame_candidate(const IR_t * nd) {
    int blob_frame_scope(void);
    if (!nd || nd->op != IR_MATCH_ARBNO || nd->n_operands < 3 || !g_emit_cfg) return 0;
    int s0 = -1, s1 = -1;
    for (int j = 0; j < g_emit_cfg->n; j++) { if (g_emit_cfg->all[j] == nd->operands[1]) s0 = j; if (g_emit_cfg->all[j] == nd->operands[2]) s1 = j; }
    if (s0 > s1) { int t = s0; s0 = s1; s1 = t; }
    if (s0 < 0) return 0;
    for (int j = s0; j <= s1; j++) { IR_t * m = g_emit_cfg->all[j]; if (!m || m == nd) continue;
        if (m->op == IR_MATCH_DEFER && !m->pat_static) return 1;
        int mo = (int)m->op;
        if (zd_k(m) != 0 || mo == IR_MATCH_DEFER || mo == IR_MATCH_ALTERNATE || mo == IR_MATCH_ARBNO || (mo == IR_MATCH_FENCE1 || mo == IR_MATCH_FENCE0) || mo == IR_MATCH_VALUE
            || mo == IR_CALL || mo == IR_CALL_VALUE || mo == IR_DISJUNCTION || mo == IR_MATCH_ABORT) return 1;    }
    if (arbno_reentry_hazard(nd)) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int sn4_span_frame(void) {
    static int _sf = -1; if (_sf < 0) { const char * e = getenv("SCRIP_SPAN_FRAME"); _sf = (e && *e == '0') ? 0 : 1; } return _sf;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int sn4_xh_frame_extra(void) {
    static int _xf = -1; if (_xf < 0) { const char * e = getenv("SCRIP_XH_FRAME_EXTRA"); _xf = (e && *e == '0') ? 0 : 1; } return _xf;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int cap_fail_retreat(void) {
    static int _cf = -1; if (_cf < 0) { const char * e = getenv("SCRIP_CAP_FAIL_RETREAT"); _cf = (e && *e == '0') ? 0 : 1; } return _cf;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int sn4_pt_frame(void) {
    static int _pf = -1; if (_pf < 0) { const char * e = getenv("SCRIP_PT_FRAME"); _pf = (e && *e == '0') ? 0 : 1; } return _pf;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int leaf_frame_member(const IR_t * nd) { extern int zdp_scratch_cell(const IR_t *); int blob_frame_scope(void); return nd && zdp_scratch_cell(nd) && ((sn4_span_frame() && alt_arm_member(nd, (const IR_t *)0)) || (sn4_pt_frame() && blob_frame_scope())); }
int sn4_pt_opframe(void) { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_PT_OPFRAME"); v = e ? (*e == '1') : sn4_pt_frame(); } return v; }
static int xop_hazard_kind(int op) { return op == IR_MATCH_DEFER || op == IR_MATCH_ARBNO || op == IR_MATCH_VALUE; }
static int zd_k(IR_t * nd);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int xop_frame_member(const IR_t * nd) {
    extern int zdp_scratch_cell(const IR_t *); int blob_frame_scope(void);
    if (!nd || !sn4_pt_opframe() || !blob_frame_scope() || zdp_scratch_cell(nd) || zd_k((IR_t *)nd) != 16) return 0;
    if (nd->op == IR_MATCH_ASSIGN_SAVE || nd->op == IR_MATCH_ARBNO || nd->op == IR_MATCH_FENCE1) return 0;
    if (!g_emit_cfg) return 0;
    for (int j = 0; j < g_emit_cfg->n; j++) { const IR_t * c = g_emit_cfg->all[j]; if (!c || c == nd) continue;
        int isop = 0; for (int k = 0; k < c->n_operands; k++) if (c->operands[k] == nd) { isop = 1; break; }
        if (!isop) continue;
        if (alt_arm_member(c, (const IR_t *)0)) return 1;
        int haz = 0; const IR_t * t = zd_chase(nd->γ.node);
        for (int s = 0; t && s < 256; s++) { if (t == c) { if (haz) return 1; break; } if (xop_hazard_kind((int)t->op)) haz = 1; t = zd_chase(t->γ.node); } }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int frame_slot_is_candidate(const IR_t * nd) {
    if (!nd) return 0;
    if (nd->op == IR_MATCH_ARBNO) return arbno_frame_candidate(nd);
    if (nd->op == IR_MATCH_ASSIGN_SAVE) return frame_need_of(nd);
    if (nd->op == IR_MATCH_FENCE1) return fence_frame_candidate(nd);
    if (leaf_frame_member(nd)) return 1;
    if (xop_frame_member(nd)) return 1;
    return 0;
}
int blob_frame_scope(void);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int frame_slot_scan(const IR_t * query, int * out_index, int * out_count) {
    if (out_index) *out_index = -1; if (out_count) *out_count = 0;
    if (!g_emit_cfg) return 0;
    int qi = -1; if (query) for (int j = 0; j < g_emit_cfg->n; j++) if (g_emit_cfg->all[j] == query) { qi = j; break; }
    int mb = -1; for (int j = (qi >= 0 ? qi : g_emit_cfg->n - 1); j >= 0; j--) { IR_t * m = g_emit_cfg->all[j]; if (m && m->op == IR_MATCH_BEGIN) { mb = j; break; } }
    int blob = (mb < 0 && blob_frame_scope()) ? 1 : 0;
    if (mb < 0 && !blob) return 0;
    int hi = g_emit_cfg->n; if (!blob) for (int j = mb + 1; j < g_emit_cfg->n; j++) { IR_t * m = g_emit_cfg->all[j]; if (m && m->op == IR_MATCH_BEGIN) { hi = j; break; } }
    int k = 0, found = -1; extern int zdp_scratch_cell(const IR_t *);
    for (int j = mb + 1; j < hi; j++) { IR_t * m = g_emit_cfg->all[j]; if (!m || !frame_slot_is_candidate(m)) continue;
        if (j == qi) found = k; k += zdp_scratch_cell(m) ? 2 : 1; }
    if (out_index) *out_index = found; if (out_count) *out_count = k;
    return blob ? 2 : 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int sn4_blob_casmark(void) { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_BLOB_CASMARK"); v = (e && *e == '0') ? 0 : 1; } return v; }
static int blob_head_bytes(void) { return sn4_blob_casmark() ? 40 : 24; }
static int frame_slot_off(int scan_rc, int idx) { return -((scan_rc == 2 ? (blob_head_bytes() + 8) : 64) + 16 * idx); }
extern "C" int sn4_choice_rbp_off(void);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int zzone_off_for(const IR_t * nd, int customer) {
    if (!nd) return -1;
    switch (customer) {
    case ZCUS_LEAF:    return leaf_frame_slot(nd);
    case ZCUS_ARBNO:   return arbno_frame_slot(nd);
    case ZCUS_CAPTURE: return capture_frame_slot(nd);
    case ZCUS_FENCE:   return fence_frame_slot(nd);
    case ZCUS_CHOICE:  { int c = sn4_choice_rbp_off(); return c ? c : -1; }
    default:           return -1;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int sn4_defer_resume(void) {
    static int _dres = -1; if (_dres < 0) { const char * e = getenv("SCRIP_DEFER_RESUME"); _dres = (e && *e == '0') ? 0 : 1; } return _dres;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int sn4_arbno_altsib(void) { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_ARBNO_ALTSIB"); v = (e && *e == '0') ? 0 : 1; } return v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int sn4_arbno_seal_omega(void) {
    const char * e = getenv("SCRIP_ARBNO_SEAL_OMEGA"); return (e && *e == '0') ? 0 : 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int sn4_arbno_tailbeta(void) {
    static int _atb = -1; if (_atb < 0) { const char * e = getenv("SCRIP_ARBNO_TAILBETA"); _atb = (e && *e == '0') ? 0 : 1; } return _atb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int sn4_seam_walk(void) {
    static int _sw = -1; if (_sw < 0) { const char * e = getenv("SCRIP_SEAM_WALK"); _sw = (e && *e == '0') ? 0 : 1; } return _sw;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int sn4_alt_carrier(void) {
    static int _ac = -1; if (_ac < 0) { const char * e = getenv("SCRIP_ALT_CARRIER"); _ac = (e && *e == '0') ? 0 : 1; } return _ac;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void sn4_blob_choice_scan(int * n_choice, int * leaf_ok, int * has_fence) {
    int _nc = 0, _lf = 1, _fn = 0;
    if (g_emit_cfg) for (int j = 0; j < g_emit_cfg->n; j++) { IR_t * m = g_emit_cfg->all[j]; if (!m) continue; int o = (int)m->op;
        if (o == IR_MATCH_ALTERNATE || o == IR_DISJUNCTION) { if (m->seal) _lf = 0; else _nc++; continue; }
        if ((o == IR_MATCH_FENCE1 || o == IR_MATCH_FENCE0)) { _fn = 1; continue; }
        if (o == IR_SUCCEED || o == IR_FAIL) continue;
        { extern int zdp_scan_pure(const IR_t *); if (!zdp_scan_pure(m)) _lf = 0; } }
    if (n_choice) *n_choice = _nc; if (leaf_ok) *leaf_ok = _lf; if (has_fence) *has_fence = _fn;
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
    return (emit_match_rbp() && g_emit.flat_jmp_entry && g_emit.flat_pat && !(g_flat_frame_floor > 0)) ? 1 : 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int blob_wire_clobber_scan(void) {
    if (!g_emit_cfg) return 0;
    for (int j = 0; j < g_emit_cfg->n; j++) { IR_t * m = g_emit_cfg->all[j]; if (!m) continue; int o = (int)m->op; if (o == IR_MATCH_DEFER || o == IR_CALL || o == IR_CALL_VALUE || o == IR_MATCH_VALUE || o == IR_MATCH_CALLOUT) return 1; }
    return 0;
}
extern "C" int sn4_choice_rbp(void);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" int sn4_choice_rbp(void) {
    static int _cr = -1; if (_cr < 0) { const char * e = getenv("SCRIP_CHOICE_RBP"); extern int sn4_pt_frame(void); _cr = e ? (*e == '1') : sn4_pt_frame(); } return _cr;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int sn4_alt_fence_relax(void) {   /* ⛔⛔ seat04 2026-08-22 MEASURED-BROKEN PROBE, DO NOT PROMOTE WITHOUT A NEW FIX: `!_fn` below was believed empirical-not-semantic (HQ, citing resume_carrier_ok tier 3 s189: `lf=0 fn=1` on BOTH ptw_min_fence_left_altresume/ptw_min_fence_alttop CURES, so FENCE presence does not discriminate THERE) and recursion-unsafety was independently measured NOT to block a fixed RBP slot for jarray/jobject (nested-array gdb witness: distinct rbp per PAT$N stored-pattern invocation, `jbig_nested.sno` `[[9]]`, 0x...dd68 outer vs 0x...da18 inner -- each gets its own push-rbp activation frame, confirmed s`2659558e`). Armed (`SCRIP_CHOICE_RBP_FENCE=1`), the json.sno `[1,2]` hang IS CURED (rc 124 -> rc 0) but the ANSWER IS WRONG (`NOMATCH`, oracle says `MATCH`) -- a silent-wrong-answer regression, worse than the hang it fixes. Ruled out: frame-slot collision (`n199`/`n241`'s identical `[rbp-184/-176/-168]` offsets are in DIFFERENT `PAT$N` blocks -- PAT$8 vs PAT$9 -- hence different physical frames, not a real collision, checked by grep across the whole `--compile` output). gdb showed the cursor (r14d) go BACKWARD (2 -> 1, not forward to 4) on what should be ARBNO's greedy extension attempt after `]` first fails at cursor 2 -- consistent with ARBNO's OWN "interior choice point af back-out edge" (`op_arbno_body_actframe`, recovered s125-127 comments, `git show e25a5daf^:src/emitter/emit.cpp`) jumping into `n241`'s ports on an assumption never verified against an RBP-resident (rather than FLAT) sibling record -- i.e. this is very likely a FRESH INSTANCE of s126's own already-named unsolved class, quoted verbatim in that recovered comment: "one-deep records compose by contiguity, two-deep do not yet." Full trace, both ruled-out hypotheses, and this hypothesis: `FINDING-2026-08-22-seat04-json-alternate-af-spin-root-cause-flat-choice-record-rsp-drift.md` §12. Default 0 (OFF), byte-identical to pre-existing behavior; DO NOT flip default-on until the ARBNO-interior/RBP-resident-record composition is actually designed and verified, not just guessed past. */
    static int _fr = -1; if (_fr < 0) { const char * e = getenv("SCRIP_CHOICE_RBP_FENCE"); _fr = (e && *e == '1') ? 1 : 0; } return _fr;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int blob_choice_rbp_scan(void) {
    if (!sn4_choice_rbp() || !sn4_alt_carrier() || !blob_frame_scope() || !g_emit_cfg) return 0;
    int _nc = 0, _lf = 0, _fn = 0; sn4_blob_choice_scan(&_nc, &_lf, &_fn);
    if (_nc != 1 || (_fn && !sn4_alt_fence_relax())) return 0;
    { extern int sn4_pt_frame(void); if (!sn4_pt_frame()) {
        if (_lf) return 0;
        if (blob_wire_clobber_scan()) return 0;
        int count = 0; if (frame_slot_scan((const IR_t *)0, (int *)0, &count) != 2) return 0;
        if (count == 0) return 0;
        return 1; } }
    if (frame_slot_scan((const IR_t *)0, (int *)0, (int *)0) != 2) return 0;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int blob_frame_bytes(void) {
    if (!blob_frame_scope() || !g_emit_cfg) return 0;
    int count = 0; if (frame_slot_scan((const IR_t *)0, (int *)0, &count) != 2) { count = 0; }
    int _pend = 0; if (sn4_blob_casmark()) for (int _i = 0; _i < g_emit_cfg->n; _i++) { IR_t * _m = g_emit_cfg->all[_i]; if (_m && _m->op == IR_MATCH_ASSIGN_COND) { _pend = 1; break; } }
    if (count == 0 && !_pend && !blob_wire_clobber_scan() && !blob_choice_rbp_scan()) return blob_head_bytes();
    return blob_head_bytes() + 16 * count + (blob_choice_rbp_scan() ? 32 : 0);
}
extern "C" int sn4_choice_rbp_off(void);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" int sn4_choice_rbp_off(void) {
    return blob_choice_rbp_scan() ? -(int)blob_frame_bytes() : 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" int emit_diag_regs_suppress(void) {
    return (g_emit.flat_jmp_entry && g_emit.flat_pat && blob_frame_bytes() <= 0) ? 1 : 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" int x86_diag_regs_on_c(void) { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_DIAG_REGS"); v = (e && *e == '0') ? 0 : 1; } return v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int arbno_frame_slot(const IR_t * arbno_nd) {
    if (!emit_match_rbp() || !arbno_frame_candidate(arbno_nd)) return -1;
    int idx = -1, rc = frame_slot_scan(arbno_nd, &idx, NULL); if (!rc || idx < 0) return -1;
    return frame_slot_off(rc, idx);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int fence_frame_slot(const IR_t * fence_nd) {
    if (!emit_match_rbp() || !fence_frame_candidate(fence_nd)) return -1;
    int idx = -1, rc = frame_slot_scan(fence_nd, &idx, NULL); if (!rc || idx < 0) return -1;
    return frame_slot_off(rc, idx);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int leaf_frame_slot(const IR_t * leaf_nd) {
    if (!emit_match_rbp() || !leaf_frame_member(leaf_nd)) return -1;
    int idx = -1, rc = frame_slot_scan(leaf_nd, &idx, NULL); if (!rc || idx < 0) return -1;
    return frame_slot_off(rc, idx + 1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int xop_frame_slot(const IR_t * nd) {
    if (!emit_match_rbp() || !xop_frame_member(nd)) return -1;
    int idx = -1, rc = frame_slot_scan(nd, &idx, NULL); if (!rc || idx < 0) return -1;
    return frame_slot_off(rc, idx);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int capture_frame_slot(const IR_t * cap_nd) {
    if (!emit_match_rbp() || !cap_nd) return -1;
    const IR_t * save = (cap_nd->op == IR_MATCH_ASSIGN_SAVE) ? cap_nd : (cap_nd->n_operands > 1 ? cap_nd->operands[1] : (IR_t *)0);
    if (!save || save->op != IR_MATCH_ASSIGN_SAVE || !frame_need_of(save)) return -1;
    int idx = -1, rc = frame_slot_scan(save, &idx, NULL); if (!rc || idx < 0) return -1;
    return frame_slot_off(rc, idx);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int emit_match_begin_frame_extra(const IR_t * match_begin_nd) {
    if (!emit_match_rbp() || !g_emit_cfg) return 0;
    int mb = -1; for (int j = 0; j < g_emit_cfg->n; j++) if (g_emit_cfg->all[j] == match_begin_nd) { mb = j; break; }
    if (mb < 0) return 0;
    int hi = g_emit_cfg->n; for (int j = mb + 1; j < g_emit_cfg->n; j++) { IR_t * m = g_emit_cfg->all[j]; if (m && m->op == IR_MATCH_BEGIN) { hi = j; break; } }
    int count = 0; for (int j = mb + 1; j < hi; j++) { IR_t * m = g_emit_cfg->all[j]; if (m && frame_slot_is_candidate(m)) count++; }
    return 16 * count;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int zd_k(IR_t * nd) { int op = (int)nd->op; if (op == IR_MATCH_ARBNO) return emit_arbno_rbp() ? 32 : 16;    if (op == IR_TO) return 32;    return (op == IR_ASSIGN || op == IR_GOTO || op == IR_GOTO_DEFERRED || op == IR_DEFINE ||    op == IR_MATCH_BEGIN || op == IR_MATCH_END || op == IR_MATCH_REPLACE || op == IR_STATEMENT || op == IR_STATEMENT_BEGIN || op == IR_STATEMENT_END || op == IR_MATCH_LIT || op == IR_MATCH_LEN || op == IR_MATCH_ANY || op == IR_MATCH_NOTANY || op == IR_MATCH_POS || op == IR_MATCH_RPOS || op == IR_MATCH_ASSIGN_COND || op == IR_MATCH_ASSIGN_IMM || op == IR_MATCH_VALUE || op == IR_MATCH_ALTERNATE || (op == IR_MATCH_FENCE1 || op == IR_MATCH_FENCE0) || op == IR_BOUND || op == IR_UNMARK || op == IR_CONJUNCTION || op == IR_CUT || op == IR_MOVE_LABEL || op == IR_GLIT || op == IR_GCC || op == IR_GALT || op == IR_RETURN || (op == IR_DISJUNCTION && nd->n_operands == 0) || (op == IR_MATCH_DEFER && nd->pat_static && IR_LIT(nd).sval && !strncmp(IR_LIT(nd).sval, "PATV$", 5))) ? 0 : 16; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void zd_plan(IR_t **nodes, int n, unsigned char *zon, int *zout, int *zgpop, int *zwpop, int *zarm) {
    extern const char * bb_src_of(const IR_t *);
    static int _zd = -1, _dg = -1; static const char * _zo; static const char * _zs;
    if (_zd < 0) { const char * e = getenv("SCRIP_ZD"); _zd = (e && *e == '0') ? 0 : 1; const char * d = getenv("SCRIP_ZD_DIAG"); _dg = (d && *d == '1') ? 1 : 0; _zo = getenv("SCRIP_ZD_ONLY"); _zs = getenv("SCRIP_ZD_SKIP"); }
    for (int i = 0; i < n; i++) { zon[i] = 0; zout[i] = -1; zgpop[i] = 0; zwpop[i] = 0; if (zarm) zarm[i] = -1; }
    { static int _lp=-1; if(_lp<0){const char*e=getenv("SCRIP_ZDLOCAL");_lp=(e&&*e=='1')?1:0;} if(_lp){ extern int is_global(const char *); for (int i=0;i<n;i++){ int o=(int)nodes[i]->op; if(o==IR_VAR||o==IR_ASSIGN){ const char*vn=IR_LIT(nodes[i]).sval; int gl = (vn && is_global(vn) && !graph_has_local(g_emit_cfg, vn)); if(!gl) fprintf(stderr,"[ZDLOCAL] %s name=%s pinned=%d fbdata=%d\n", bb_op_name(nodes[i]->op), vn?vn:"<null>", x86_fb_pinned(), x86_fb_data()); } } } }
    {
      if (!_zd || (x86_port_mode() != ZC_PORT_FORTH && x86_port_mode() != ZC_PORT_HEAP) || n <= 0) return; }
    { if (g_emit_cfg && g_emit_cfg->icn_cells_graph) { for (int _bi = 0; _bi < n; _bi++) { IR_t * _bgt = zd_chase(nodes[_bi]->γ.node); IR_t * _bot = zd_chase(nodes[_bi]->ω.node); for (int _bj = 0; _bj <= _bi; _bj++) { if (nodes[_bj] == _bgt || nodes[_bj] == _bot) return; } } } }
    int * claim = (int *)alloca(sizeof(int) * (size_t)n); int * run = (int *)alloca(sizeof(int) * (size_t)n); int * rpos = (int *)alloca(sizeof(int) * (size_t)n); unsigned char * aent = (unsigned char *)alloca((size_t)n);
    unsigned char * cm = (unsigned char *)alloca((size_t)n); int * wl = (int *)alloca(sizeof(int) * (size_t)n);
    for (int i = 0; i < n; i++) { claim[i] = -1; rpos[i] = -1; }
    for (int hi = 0; hi < n; hi++) {
        if (!(hi == 0 || bb_src_of(nodes[hi]))) continue;
        if (claim[hi] >= 0) continue;
        int rl = 0; IR_t * cur = nodes[hi]; int guard = 0;
        static int _z5b = -1; if (_z5b < 0) { const char * e = getenv("SCRIP_ZD_5B"); _z5b = (e && *e == '0') ? 0 : 1; }
        while (cur && guard++ <= n) {
            int ci = -1; for (int k = 0; k < n; k++) if (nodes[k] == cur) { ci = k; break; }
            if (ci < 0 || claim[ci] >= 0) break;
            if (rl > 0 && bb_src_of(nodes[ci])) break;
            { static int _fl = -1; if (_fl < 0) { const char * e = getenv("SCRIP_ZD_FLOATER_CUT"); _fl = (e && *e == '0') ? 0 : 1; } if (_fl && rl > 0 && emit_floater_kind(nodes[ci])) break; }
            run[rl] = ci; rpos[ci] = rl; claim[ci] = hi; rl++;
            cur = zd_chase(cur->γ.node);
        }
        { static int _z5bd = -1; if (_z5bd < 0) { const char * e = getenv("SCRIP_ZD_5B"); _z5bd = (e && *e == '0') ? 0 : 1; }
          for (int k = 0; k < n; k++) aent[k] = 0;
          if (_z5bd && zarm) { int rl_main = rl;
            for (int r0 = 0; r0 < rl_main; r0++) { IR_t * env = nodes[run[r0]]; if ((int)env->op != IR_MATCH_ALTERNATE) continue; int ei = run[r0];
                for (int a = 0; a + 1 < env->n_operands; a += 2) { IR_t * c2 = env->operands[a]; int rl0 = rl; int bad = 0; int g2 = 0; int first = 1;
                    while (c2 && g2++ <= n) {
                        int ci = -1; for (int k = 0; k < n; k++) if (nodes[k] == c2) { ci = k; break; }
                        if (ci < 0 || claim[ci] >= 0) break;
                        if (bb_src_of(nodes[ci]) || emit_floater_kind(nodes[ci])) { bad = 1; break; }
                        { int aop = (int)nodes[ci]->op; int leaf = (aop == IR_MATCH_LIT || aop == IR_MATCH_LEN || aop == IR_MATCH_ANY || aop == IR_MATCH_NOTANY || aop == IR_MATCH_SPAN || aop == IR_MATCH_BREAK || aop == IR_MATCH_BREAKX || aop == IR_MATCH_POS || aop == IR_MATCH_RPOS || aop == IR_MATCH_TAB || aop == IR_MATCH_RTAB || aop == IR_MATCH_ARB || aop == IR_MATCH_REM || aop == IR_MATCH_BAL);
                          if (!leaf) { bad = 1; break; }
                          if (nodes[ci]->n_operands > 0) { run[rl] = ci; rpos[ci] = rl; claim[ci] = hi; zarm[ci] = ei; if (first) { aent[ci] = 1; first = 0; } rl++; } }
                        c2 = zd_chase(c2->γ.node);
                    }
                    if (bad) { for (int u = rl0; u < rl; u++) { int ci = run[u]; claim[ci] = -1; rpos[ci] = -1; zarm[ci] = -1; aent[ci] = 0; } rl = rl0; }
                } } } }
        { static int _zvo = -1; if (_zvo < 0) { const char * e = getenv("SCRIP_ZD_VLIST_OMEGA"); _zvo = (e && *e == '1') ? 1 : 0; }
          if (_zvo && zarm) { int grew = 1;
            while (grew) { grew = 0; int rl_now = rl;
                for (int r0 = 0; r0 < rl_now; r0++) { int i0 = run[r0];
                    IR_t * ot = zd_chase(nodes[i0]->ω.node); if (!ot) continue;
                    int oti = -1; for (int k = 0; k < n; k++) if (nodes[k] == ot) { oti = k; break; }
                    if (oti < 0 || claim[oti] >= 0 || bb_src_of(nodes[oti])) continue;
                    int seed = (zarm[i0] >= 0) ? zarm[i0] : i0;
                    int rl0 = rl; int bad = 0; int g2 = 0; int first = 1; IR_t * c2 = ot;
                    while (c2 && g2++ <= n) {
                        int ci = -1; for (int k = 0; k < n; k++) if (nodes[k] == c2) { ci = k; break; }
                        if (ci < 0 || claim[ci] >= 0) break;
                        if (bb_src_of(nodes[ci]) || emit_floater_kind(nodes[ci])) { bad = 1; break; }
                        run[rl] = ci; rpos[ci] = rl; claim[ci] = hi; zarm[ci] = seed; if (first) { aent[ci] = 1; first = 0; } rl++;
                        c2 = zd_chase(c2->γ.node);
                    }
                    if (bad) { for (int u = rl0; u < rl; u++) { int ci = run[u]; claim[ci] = -1; rpos[ci] = -1; zarm[ci] = -1; aent[ci] = 0; } }
                    else if (rl > rl0) grew = 1;
                } } } }
        for (int k = 0; k < n; k++) cm[k] = 0;
        int nblob = 0;
        { int wn = 0;
          for (int r = 0; r < rl; r++) { cm[run[r]] = 1; wl[wn++] = run[r]; }
          while (wn > 0) { int i = wl[--wn];
              for (int a = 0; a < nodes[i]->n_operands; a++) { IR_t * p = nodes[i]->operands[a]; if (!p) continue;
                  for (int k = 0; k < n; k++) if (nodes[k] == p && !cm[k]) { cm[k] = 1; wl[wn++] = k; nblob++; break; } } } }
        int ok = (rl > 0); const char * why = ""; int badi = -1; int rgood = rl;
        for (int r = 0; r < rl && ok; r++) { int i = run[r];
            { int _a = zd_wl_kind(nodes[i]);
              if (!_a) { ok = 0; why = bb_op_name(nodes[i]->op); badi = i; rgood = r; break; } }
            if ((_zo && !zw_nid_listed(_zo, i)) || (_zs && zw_nid_listed(_zs, i))) { ok = 0; why = "nidgate"; badi = i; rgood = r; break; }
            { int no = zd_nops(nodes[i]); if (nodes[i]->n_operands < no) { ok = 0; why = "nops"; badi = i; rgood = r; break; }
              for (int j = 0; j < no; j++) { IR_t * p = nodes[i]->operands[j]; int f = -1; for (int k = 0; k < rl; k++) if (nodes[run[k]] == p) { f = k; break; }
                  if (f < 0 || f >= r) { ok = 0; why = "opnd"; badi = i; rgood = r; break; } } }
        }
        { if (ok && g_emit_cfg && g_emit_cfg->icn_cells_graph) { for (int _rb = 0; _rb < rl && ok; _rb++) { IR_t *_gt = zd_chase(nodes[run[_rb]]->γ.node); if (!_gt) continue; for (int _rg = 0; _rg < _rb && ok; _rg++) { if (nodes[run[_rg]] == _gt) { ok = 0; why = ir_is_generator_kind(nodes[run[_rg]]->op) ? "gen-loop-body" : "loop-backredge"; badi = run[_rg]; rgood = _rg; } } } } }
        (void)rgood;
        if (ok) { int zd = 0; int arm_zd = 0; int zdh_match = -1;
            for (int r = 0; r < rl; r++) { int i = run[r];
                int REL = fence0_release_bytes(nodes[i]);
                int K = zd_k(nodes[i]);
                { if (nodes[i]->op == IR_MATCH_BEGIN && zdh_match < 0) zdh_match = (int)zd + K; }
                if (zarm && zarm[i] >= 0) { if (aent[i]) arm_zd = zout[zarm[i]] - zd_k(nodes[zarm[i]]); zon[i] = 1; zout[i] = arm_zd + K; arm_zd = arm_zd + K; }
                else { zon[i] = 1; zout[i] = zd + K - REL; zd = zd + K - REL; }
                IR_t * gt = zd_chase(nodes[i]->γ.node); IR_t * ot = zd_chase(nodes[i]->ω.node);
                int gin = 0; int oin = 0;
                int gib = port_sz_beta(nodes[i]->γ.sz); { int _gg = 0; IR_t * _g = nodes[i]->γ.node; while (_g && _g->op == IR_GOTO && _gg++ < 128) { if (!gib) gib = port_sz_beta(_g->γ.sz); _g = _g->γ.node; } } gib = gib && !beta_is_stmt_land(gt);
                if (nblob > 0) { for (int k = 0; k < n; k++) if (cm[k]) { if (nodes[k] == gt) gin = 1; if (nodes[k] == ot && !(port_sz_beta(nodes[i]->ω.sz) && beta_is_stmt_land(ot))) oin = 1; } }
                else for (int k = 0; k < rl; k++) { if (nodes[run[k]] == gt && (k > r || gib)) gin = 1; if (nodes[run[k]] == ot && !(port_sz_beta(nodes[i]->ω.sz) && beta_is_stmt_land(ot))) oin = 1; }
                { if (!gin && gt && gt->op == IR_SUCCEED && g_emit_cfg && g_emit_cfg->icn_cells_graph && port_sz_beta(nodes[i]->ω.sz)) gin = 1; }
                { (void)0; }
                { int oib = port_sz_beta(nodes[i]->ω.sz); if (!oin && oib && K == 0 && !beta_is_stmt_land(ot)) oin = 1; }
                { if (!oin && K == 0) { int _io = !ot; if (!_io) { for (int _ik = 0; _ik <= r; _ik++) { if (nodes[run[_ik]] == ot) { _io = 1; break; } } } if (_io) oin = 1; } }
                                { long kc = 0;
                int _wzdepth = (int)zd;
                {
                if (!gin) zgpop[i] = ((zdh_match >= 0 && (nodes[i]->op == IR_STATEMENT_END || nodes[i]->op == IR_STATEMENT)) ? zdh_match + emit_match_begin_stfh_k() : _wzdepth) + (int)kc;
                if (!oin) zwpop[i] = _wzdepth - K + (int)kc; } }
                if (_dg) fprintf(stderr, "[ZD] h=%d r=%d i=%d %s K=%d zout=%d gpop=%d wpop=%d\n", hi, r, i, bb_op_name(nodes[i]->op), K, zout[i], zgpop[i], zwpop[i]);
            }
        }
        else {
            for (int r = 0; r < rl; r++) rpos[run[r]] = -1;
               if (_dg && rl > 0) fprintf(stderr, "[ZD] run h=%d len=%d REFUSED at i=%d (%s op=%d)\n", hi, rl, badi, why, badi >= 0 ? (int)nodes[badi]->op : -1);
               { static int _gp = -1; if (_gp < 0) { const char * _e = getenv("SCRIP_ZD_GAP"); _gp = (_e && _e[0] == '1') ? 1 : 0; } if (_gp && rl > 0) { for (int _r = 0; _r < rl; _r++) { IR_t * _n = nodes[run[_r]]; const char * _nm = bb_op_name(_n->op); { int _o = (int)_n->op; int _ic = (_o == (int)IR_CALL || (_o != (int)IR_CALL_VALUE && ir_is_call_kind((IR_e)_o))); const char * _cs = (_ic && IR_LIT(_n).sval) ? IR_LIT(_n).sval : "-"; fprintf(stderr, "[ZD-GAP] r=%d op=%s(%d) admit=%d nops=%d callee=%s\n", _r, _nm ? _nm : "<unnamed>", _o, zd_wl_kind(_n) ? 1 : 0, (int)_n->n_operands, _cs); } } } }
               { static int _cd = -1; if (_cd < 0) { const char *_e = getenv("SCRIP_CALL_DIAG"); _cd = (_e && _e[0] == '1') ? 1 : 0; }
                 if (_cd && rl > 0 && badi >= 0) { IR_t * _bn = nodes[badi]; int _bop = (int)_bn->op; int _is_call = (_bop == (int)IR_CALL || (_bop != (int)IR_CALL_VALUE && ir_is_call_kind((IR_e)_bop))); const char * _callee = (_is_call && IR_LIT(_bn).sval) ? IR_LIT(_bn).sval : (const char *)0; fprintf(stderr, "[CALL-DIAG] REFUSE blocker op=%s(%d) callee=%s rl=%d badi=%d narg=%d\n", bb_op_name(_bn->op), _bop, _callee ? _callee : "-", rl, badi, _is_call ? (int)_bn->n_operands : -1); } } }
    }
}
typedef struct { IR_t * t; int d0; int nd; int multi; } zdw_ent_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void zd_depth_census(IR_t **nodes, int n, unsigned char *zon, int *zout, int *zgpop, int *zwpop, const char *prefix) {
    static int _on = -1; if (_on < 0) { const char * e = getenv("SCRIP_ZD_DEPTH"); _on = (e && *e == '1') ? 1 : 0; }
    if (!_on || n <= 0) return;
    zdw_ent_t * w = (zdw_ent_t *)calloc((size_t)(2 * n + 1), sizeof(zdw_ent_t)); if (!w) return;
    int wn = 0, armed = 0, edges = 0, walls = 0;
    for (int i = 0; i < n; i++) { if (!zon[i]) continue; armed++;
        int K = zd_k(nodes[i]);
        for (int p = 0; p < 2; p++) { IR_t * t = zd_chase(p == 0 ? nodes[i]->γ.node : nodes[i]->ω.node); if (!t) continue;
            int d = (p == 0) ? (zout[i] - zgpop[i]) : (zout[i] - K - zwpop[i]); int ti = -1;
            for (int k = 0; k < n; k++) if (nodes[k] == t) { ti = k; break; }
            if (ti < 0 || !zon[ti]) continue; edges++;
            int f = -1; for (int k = 0; k < wn; k++) if (w[k].t == t) { f = k; break; }
            if (f < 0) { w[wn].t = t; w[wn].d0 = d; w[wn].nd = 1; w[wn].multi = 0; wn++; }
            else { w[f].nd++; if (d != w[f].d0 && !w[f].multi) { w[f].multi = 1; walls++; } } } }
    for (int k = 0; k < wn; k++) { if (!w[k].multi) continue;
        fprintf(stderr, "[ZD-DEPTH] WALL %s %s preds=%d first_depth=%d\n", prefix ? prefix : "?", bb_op_name(w[k].t->op) ? bb_op_name(w[k].t->op) : "<unnamed>", w[k].nd, w[k].d0);
        for (int i = 0; i < n; i++) { if (!zon[i]) continue; int Kp = zd_k(nodes[i]);
            for (int p = 0; p < 2; p++) { IR_t * t = zd_chase(p == 0 ? nodes[i]->γ.node : nodes[i]->ω.node); if (t != w[k].t) continue;
                int d = (p == 0) ? (zout[i] - zgpop[i]) : (zout[i] - Kp - zwpop[i]);
                fprintf(stderr, "[ZD-DEPTH]   pred %s %-24s depth=%d%s\n", p == 0 ? "\xce\xb3" : "\xcf\x89", bb_op_name(nodes[i]->op) ? bb_op_name(nodes[i]->op) : "<unnamed>", d, d == w[k].d0 ? "" : "   <-- DISAGREES"); } } }
    fprintf(stderr, "[ZD-DEPTH] %s n=%d armed=%d in_edges=%d joins=%d walls=%d\n", prefix ? prefix : "?", n, armed, edges, wn, walls);
    free(w);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void flat_beta_used_scan(IR_t **nodes, int n, unsigned char *used) {
    for (int j = 0; j < n; j++) if (fc_seq_on(nodes[j]) || fc_alt_active(nodes[j])) { for (int k = 0; k < n; k++) used[k] = 1; return; }
    for (int k = 0; k < n; k++) {
        int op = (int)nodes[k]->op;
        used[k] = (ir_is_generator_kind(nodes[k]->op) || op == IR_SUSPEND || op == IR_CALL || op == IR_CALL_PROC_STAGED || op == IR_CALL_BUILTIN_GEN || op == IR_PROC_GEN || op == IR_REPALT
                   || op == IR_LIMIT || op == IR_GOTO || op == IR_STATEMENT_BEGIN
                   || (g_emit_cfg && nodes[k] == g_emit_cfg->body_root)) ? 1 : 0;
    }
    for (int j = 0; j < n; j++) {
        int op = (int)nodes[j]->op;
        if (op == IR_MATCH_ALTERNATE || op == IR_MATCH_ARBNO || (op == IR_MATCH_FENCE1 || op == IR_MATCH_FENCE0) || op == IR_SCAN_SEQUENCE || op == IR_SCAN_ALTERNATE || op == IR_DISJUNCTION || op == IR_REPALT)
            for (int a = 0; a < nodes[j]->n_operands; a++) for (int k = 0; k < n; k++) if (nodes[k] == nodes[j]->operands[a]) used[k] = 1;
        if (op == IR_MOVE_LABEL && nodes[j]->n_operands > 0 && nodes[j]->operands[0] && (int)IR_LIT(nodes[j]).ival)
            for (int k = 0; k < n; k++) if (nodes[k] == nodes[j]->operands[0]) used[k] = 1;
        int gib = port_sz_beta(nodes[j]->γ.sz);
        int oib = port_sz_beta(nodes[j]->ω.sz);
        IR_t *gtgt = nodes[j]->γ.node;
        IR_t *otgt = nodes[j]->ω.node;
        { int _gg = 0; IR_t * _g = gtgt; while (_g && _g->op == IR_GOTO && _gg++ < 128) { if (!gib) gib = port_sz_beta(_g->γ.sz); gtgt = _g->γ.node; _g = gtgt; } }
        { int _gg = 0; IR_t * _o = otgt; while (_o && _o->op == IR_GOTO && _gg++ < 128) { if (!oib) oib = port_sz_beta(_o->γ.sz); otgt = _o->γ.node; _o = otgt; } }
        if (gib) for (int k = 0; k < n; k++) if (nodes[k] == gtgt) used[k] = 1;
        if (oib) for (int k = 0; k < n; k++) if (nodes[k] == otgt) used[k] = 1;
    }
}
extern "C" int xa_flat_class_c_pred(void);
extern "C" void xa_flat_chain_prologue(void);
extern "C" void xa_flat_chain_epilogue(void);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int codegen_flat_chain_body(IR_t *entry, const char *prefix) {
    bb_label_t lbl_α, lbl_α_body, lbl_γ, lbl_ω, lbl_β, lbl_res;
    int _top_hoist = 0;
    int _flt_hoisted[4] = {0, 0, 0, 0};
    int _flt_uid_burn[4] = {0, 0, 0, 0};
    const char *fam = (strncmp(prefix, "proc_", 5) == 0) ? prefix + 5 : prefix;
    emit_label_initf(&lbl_α,      "%s_α",      fam);
    emit_label_initf(&lbl_α_body, "%s_α_body", fam);
    emit_label_initf(&lbl_γ,       "%s_γ",      fam);
    emit_label_initf(&lbl_ω,       "%s_ω",      fam);
    emit_label_initf(&lbl_β,       "%s_β",       fam);
    emit_label_initf(&lbl_res,     "%s_res",     fam);
    int bare = g_emit.flat_bare_chain;
    if (strncmp(prefix, "proc_LBL__", 10) == 0) emit_label_initf(&lbl_α, "%s", prefix + 5);
    else if (strncmp(prefix, "proc_", 5) == 0 && !bare) emit_label_initf(&lbl_α, "FN__%s", prefix + 5);
    int fn_face_dead = bare && strncmp(prefix, "proc_", 5) == 0 && strncmp(prefix, "proc_LBL__", 10) != 0;
    if (bare) { emit_label_initf(&lbl_γ, "main_\xcf\x89"); emit_label_initf(&lbl_ω, "main_\xcf\x89"); emit_label_initf(&lbl_β, "main_\xcf\x89"); emit_label_initf(&lbl_res, "main_\xcf\x89"); }
    int text_externalise = g_is_text ? 1 : 0;
    if (text_externalise) data_buf_reset();
    g_emit.flat_lbl_α        = lbl_α.name;
    g_emit.flat_lbl_α_body   = lbl_α_body.name;
    g_emit.flat_lbl_γ         = lbl_γ.name;
    g_emit.flat_lbl_ω         = lbl_ω.name;
    g_emit.flat_lbl_β         = lbl_β.name;
    g_emit.flat_β_p           = &lbl_β;
    g_emit.flat_res_p         = &lbl_res;
    g_emit.flat_succ_p        = &lbl_γ;
    g_emit.flat_fail_p        = &lbl_ω;
    g_emit.flat_text_externalise = text_externalise;
    g_resumable_callable_active = (g_emit_cfg && g_emit_cfg->resumable_callable) ? 1 : 0;
    enum { CH_MAX = 65536, Q_MAX = CH_MAX * 16 };
    static IR_t *nodes[CH_MAX]; int n = 0;
    static IR_t *queue[Q_MAX]; int qh = 0, qt = 0;
    int entry_is_own_graph_root = (g_emit_cfg && (entry == g_emit_cfg->entry)) ? 1 : 0;
    { int guard = 0; while (entry && (entry->op == IR_SUCCEED || entry->op == IR_FAIL || entry->op == IR_GOTO) && entry->γ.node && guard++ < CH_MAX) entry = entry->γ.node; }
    if (g_emit_cfg && entry == g_emit_cfg->entry) entry_is_own_graph_root = 1;
    int flat_empty_body_fail = (entry && entry->op == IR_FAIL) ? 1 : 0;
    int flat_empty_body_succ = (entry && entry->op == IR_SUCCEED) ? 1 : 0;
    entry = entry;
    static IR_t *postv[CH_MAX]; int pn = 0;
    ir_pset_t seenset; ir_pset_init(&seenset);
#define RPO_VISITED(p) (ir_pset_has(&seenset, (const IR_t *)(p)))
#define RPO_MARK(p)    ir_pset_add(&seenset, (const IR_t *)(p))
#define RPO_TAG_EMIT   ((IR_t *)1)
#define RPO_PUSH(p)    do { if ((p) && (p)->op != IR_SUCCEED && (p)->op != IR_FAIL && !emit_floater_member(p) && !(_stmt_seed && (p)->op == IR_STATEMENT_BEGIN) && !RPO_VISITED(p) && qt < Q_MAX) queue[qt++] = (IR_t *)(p); } while (0)
#define RPO_PUSH_SUCCS(c) \
         \
        RPO_PUSH((c)->γ.node); \
        if (((c)->op == IR_MATCH_ALTERNATE || (c)->op == IR_MATCH_ARBNO || (c)->op == IR_MATCH_FENCE1 || (c)->op == IR_SCAN_SEQUENCE || (c)->op == IR_SCAN_ALTERNATE || (c)->op == IR_DISJUNCTION || (c)->op == IR_GALT) && (c)->n_operands > 0) for (int _oi = (c)->n_operands - 1; _oi >= 0; _oi--) RPO_PUSH((c)->operands[_oi]); \
        if ((ir_is_matcher_element((c)->op)) || (c)->op == IR_MATCH_DEFER || (c)->op == IR_MATCH_VALUE) RPO_PUSH((c)->ω.node); \
        if ((c)->op == IR_REPALT && (c)->n_operands > 1) RPO_PUSH((c)->operands[1]); \
        if ((c)->op == IR_REPALT && (c)->n_operands > 0) RPO_PUSH((c)->operands[0]); \
        if ((c)->op == IR_MOVE_LABEL && (c)->n_operands > 1) RPO_PUSH((c)->operands[1]); \
        if ((c)->op == IR_MOVE_LABEL && (c)->n_operands > 0) RPO_PUSH((c)->operands[0]); \
        if ((c)->op == IR_CREATE && (c)->n_operands > 0) RPO_PUSH((c)->operands[0]); \
        if ((c)->op == IR_SUSPEND && (c)->n_operands > 1) RPO_PUSH((c)->operands[1]); \
        if (((c)->op == IR_SUBSCRIPT || (c)->op == IR_RANDOM || (c)->op == IR_DEREF || (c)->op == IR_ASSIGN_VAR || (c)->op == IR_REV_ASSIGN_VAR || (c)->op == IR_KEYWORD_ASSIGN || (c)->op == IR_SCAN_TAB || (c)->op == IR_SCAN_MOVE || (c)->op == IR_SCAN_POS || (c)->op == IR_SCAN_MATCH || (c)->op == IR_SCAN_ANY || (c)->op == IR_SWAP_VAR || (c)->op == IR_CALL_VALUE || (c)->op == IR_VAR) && (c)->ω.node) RPO_PUSH((c)->ω.node); \
        if (((c)->op == IR_CALL || ir_is_call_kind((c)->op) || (c)->op == IR_PROC_GEN || (c)->op == IR_ACTIVATE || (c)->op == IR_TO || (c)->op == IR_TO_BY) && (c)->ω.node) RPO_PUSH((c)->ω.node); \
        if ((c)->op == IR_STATEMENT_BEGIN && (c)->ω.node) RPO_PUSH((c)->ω.node);  \
        if (((c)->op == IR_BINOP || (c)->op == IR_BINOP_TEST || (c)->op == IR_BINOP_RELOP_VAL || (c)->op == IR_UNOP || (c)->op == IR_UNOP_TEST || (c)->op == IR_NULLTEST_VAR || (c)->op == IR_COERCE_STRING || (c)->op == IR_COERCE_INTEGER || (c)->op == IR_COERCE_NUMERIC || (c)->op == IR_COERCE_REAL || (c)->op == IR_CMP_TEST || (c)->op == IR_IDENT || (c)->op == IR_DIFFER) && (c)->ω.node) RPO_PUSH((c)->ω.node); \
        (void)0;
#define RPO_DRAIN() \
    while (qt > qh) { \
        IR_t *c = queue[--qt]; \
        if (c == RPO_TAG_EMIT) { IR_t *e = queue[--qt]; if (pn < CH_MAX) postv[pn++] = e; continue; } \
        if (!c || c->op == IR_SUCCEED || c->op == IR_FAIL) continue; \
        if (RPO_VISITED(c)) continue; \
        RPO_MARK(c); \
        if (pn >= CH_MAX) { fprintf(stderr, "[GZ-7] FATAL chain exceeds CH_MAX\n"); abort(); } \
        if (qt + 2 > Q_MAX) { fprintf(stderr, "[GZ-7] FATAL: chain traversal queue saturated at EMIT tag\n"); abort(); } \
        queue[qt++] = c; queue[qt++] = RPO_TAG_EMIT; \
        RPO_PUSH_SUCCS(c); \
    }
#define RPO_FLUSH() do { for (int _i = pn - 1; _i >= 0; _i--) { if (n >= CH_MAX) { fprintf(stderr, "[GZ-7] FATAL chain exceeds CH_MAX\n"); abort(); } nodes[n++] = postv[_i]; } pn = 0; } while (0)
    int _stmt_seed = 0; if (g_emit_cfg && entry_is_own_graph_root) for (int _ss = 0; _ss < g_emit_cfg->n; _ss++) { IR_t * _sn = g_emit_cfg->all[_ss]; if (_sn && _sn->op == IR_STATEMENT_BEGIN) { _stmt_seed = 1; break; } }
    if (_stmt_seed) { if (qt < Q_MAX) queue[qt++] = entry; RPO_DRAIN(); RPO_FLUSH(); for (int _ss = 0; _ss < g_emit_cfg->n; _ss++) { IR_t * _sb = g_emit_cfg->all[_ss]; if (!_sb || _sb->op != IR_STATEMENT_BEGIN || RPO_VISITED(_sb)) continue; if (qt < Q_MAX) { queue[qt++] = _sb; RPO_DRAIN(); RPO_FLUSH(); } } }
    else { RPO_PUSH(entry); RPO_DRAIN(); RPO_FLUSH(); }
    { extern int zls_g_group_count(const IR_graph_t *); extern const IR_t * zls_g_group_anchor(const IR_graph_t *, int);
      if (g_emit_cfg && entry_is_own_graph_root && !_stmt_seed) { int _gc = zls_g_group_count(g_emit_cfg); for (int _k = 0; _k < _gc; _k++) { const IR_t * _a = zls_g_group_anchor(g_emit_cfg, _k); if (_a && !emit_chain_entry_already_emitted(_a)) { RPO_PUSH(_a); RPO_DRAIN(); RPO_FLUSH(); } } } }
    { if (g_emit_cfg && entry_is_own_graph_root) { int _refd[4] = {0, 0, 0, 0};
          for (int _fi = 0; _fi < g_emit_cfg->n; _fi++) { IR_t * _fn = g_emit_cfg->all[_fi]; int _fk = emit_floater_kind(_fn); if (!_fk || _refd[_fk]) continue;
              for (int _rj = 0; _rj < g_emit_cfg->n && !_refd[_fk]; _rj++) { IR_t * _r = g_emit_cfg->all[_rj]; if (_r && _r != _fn && !emit_floater_member(_r) && (_r->γ.node == _fn || _r->ω.node == _fn)) _refd[_fk] = 1; } }
          if (_refd[3]) _refd[1] = _refd[2] = 1;
          for (int _fk = 1; _fk <= 3; _fk++) { if (!_refd[_fk]) continue; for (int _fi = 0; _fi < g_emit_cfg->n; _fi++) { IR_t * _fn = g_emit_cfg->all[_fi]; if (!_fn || emit_floater_kind(_fn) != _fk || RPO_VISITED(_fn)) continue;
              if (qt < Q_MAX) { queue[qt++] = _fn; RPO_DRAIN(); RPO_FLUSH(); } } } } }
    { int _p2_base = 0; do { int _p2_added = 0; for (int i = _p2_base; i < n; i++) if (ir_is_generator_kind(nodes[i]->op) && nodes[i]->ω.node && !RPO_VISITED(nodes[i]->ω.node)) { RPO_PUSH(nodes[i]->ω.node); _p2_added = 1; } _p2_base = n; RPO_DRAIN(); RPO_FLUSH(); if (!_p2_added) break; } while (1); }
#undef RPO_FLUSH
#undef RPO_DRAIN
#undef RPO_PUSH_SUCCS
#undef RPO_PUSH
#undef RPO_TAG_EMIT
#undef RPO_MARK
#undef RPO_VISITED
    ir_pset_free(&seenset);
    if (qt >= Q_MAX) { fprintf(stderr, "[GZ-7] FATAL: chain traversal queue saturated (qt=%d >= Q_MAX=%d) for prefix=%s -- control-flow edges were silently dropped; raise CH_MAX\n", qt, (int)Q_MAX, prefix); abort(); }
    unsigned char *zd_on = (unsigned char *)alloca((size_t)(n > 0 ? n : 1)); int *zd_out = (int *)alloca(sizeof(int) * (size_t)(n > 0 ? n : 1)); int *zd_gp = (int *)alloca(sizeof(int) * (size_t)(n > 0 ? n : 1)); int *zd_wp = (int *)alloca(sizeof(int) * (size_t)(n > 0 ? n : 1)); int *zd_arm = (int *)alloca(sizeof(int) * (size_t)(n > 0 ? n : 1));
    if (!g_emit.flat_jmp_entry) g_emit.flat_pat = 0;
    zd_plan(nodes, n, zd_on, zd_out, zd_gp, zd_wp, zd_arm);
    zd_depth_census(nodes, n, zd_on, zd_out, zd_gp, zd_wp, prefix);
    {
    static int _zk0_on = -1;
    if (_zk0_on < 0) { const char * _e = getenv("SCRIP_ZD_CENSUS"); _zk0_on = (_e && *_e == '1') ? 1 : 0; }
    if (_zk0_on && g_emit_cfg && g_emit_cfg->icn_cells_graph) {
        long _k_total = 0; int _armed = 0; int _unpriced_runs = 0;
        int _blocker_op[512]; int _blocker_cnt[512]; int _n_blockers = 0;
        int _globals_on_stack = 0;
        { extern int is_global(const char *); for (int _i = 0; _i < n; _i++) { if (zd_on[_i]) { _k_total += zd_k(nodes[_i]); _armed++; int _op5 = (int)nodes[_i]->op; if ((_op5 == IR_VAR || _op5 == IR_ASSIGN) && IR_LIT(nodes[_i]).sval && is_global(IR_LIT(nodes[_i]).sval) && !graph_has_local(g_emit_cfg, IR_LIT(nodes[_i]).sval)) _globals_on_stack++; } } }
        for (int _i = 0; _i < n; _i++) {
            if (zd_on[_i]) continue;
            if (!zd_wl_kind(nodes[_i])) {
                int _op = (int)nodes[_i]->op; int _found = 0;
                for (int _b = 0; _b < _n_blockers; _b++) if (_blocker_op[_b] == _op) { _blocker_cnt[_b]++; _found = 1; break; }
                if (!_found && _n_blockers < 512) { _blocker_op[_n_blockers] = _op; _blocker_cnt[_n_blockers++] = 1; }
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
    static int _plzk0_on = -1;
    if (_plzk0_on < 0) { const char * _e = getenv("SCRIP_ZD_CENSUS"); _plzk0_on = (_e && *_e == '1') ? 1 : 0; }
    if (_plzk0_on && g_emit_cfg && g_emit_cfg->pl_cells_graph) {
        long _k_total = 0; int _armed = 0; int _refused_nodes = 0;
        int _blocker_op[512]; int _blocker_cnt[512]; int _n_blockers = 0;
        for (int _i = 0; _i < n; _i++) {
            if (zd_on[_i]) { _k_total += zd_k(nodes[_i]); _armed++; }
        }
        for (int _i = 0; _i < n; _i++) {
            if (zd_on[_i]) continue;
            if (!zd_wl_kind(nodes[_i])) {
                int _op = (int)nodes[_i]->op; int _found = 0;
                for (int _b = 0; _b < _n_blockers; _b++) if (_blocker_op[_b] == _op) { _blocker_cnt[_b]++; _found = 1; break; }
                if (!_found && _n_blockers < 512) { _blocker_op[_n_blockers] = _op; _blocker_cnt[_n_blockers++] = 1; }
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
    enum { ZW5_POOL = 128, ZW5_MAX_DEPTHS = 8 };
    bb_label_t zw5_pool[ZW5_POOL]; int zw5_base = 0;
    int zw5_stmt_idx[ZW5_POOL]; int zw5_stmt_depths[ZW5_POOL][ZW5_MAX_DEPTHS]; int zw5_stmt_cnt[ZW5_POOL]; int zw5_pool_stmts = 0;
    static int _zw5_on = -1; if (_zw5_on < 0) { const char *_e = getenv("SCRIP_ZW5"); _zw5_on = (_e && *_e == '1') ? 1 : 0; }
    if (_zw5_on) { for (int _i = 0; _i < n; _i++) { if (nodes[_i]->op != IR_STATEMENT || !zd_on[_i]) continue; int _stno = (int)IR_LIT(nodes[_i]).ival; int _depths[ZW5_MAX_DEPTHS]; int _ndepths = 0; for (int _j = _i - 1; _j >= 0; _j--) { if (zd_on[_j] && zd_wp[_j] > 0) { int _w = zd_wp[_j]; int _dup = 0; for (int _d = 0; _d < _ndepths; _d++) if (_depths[_d] == _w) { _dup = 1; break; } if (!_dup && _ndepths < ZW5_MAX_DEPTHS) _depths[_ndepths++] = _w; } if (nodes[_j]->op == IR_STATEMENT) break; }
        if (_ndepths == 0 || zw5_base + _ndepths > ZW5_POOL) continue;
        int _slot = zw5_pool_stmts++; zw5_stmt_idx[_slot] = _i; zw5_stmt_cnt[_slot] = _ndepths;
        for (int _d = 0; _d < _ndepths; _d++) { zw5_stmt_depths[_slot][_d] = _depths[_d]; emit_label_initf(&zw5_pool[zw5_base + _d], "%s_zw5s%d_ω_d%d", fam, _stno, _depths[_d]); }
        zw5_base += _ndepths; } }
    { g_emit.flat_all_zd = 0; { static int _zpg = -1; if (_zpg < 0) { const char *_e = getenv("SCRIP_ZD_PL_GEN"); _zpg = (_e && *_e == '0') ? 0 : 1; } int _pl_cells = (_zpg && g_emit_cfg && g_emit_cfg->pl_cells_graph); int _gen_ok = (!g_emit.flat_gen) || _pl_cells; int _jmp_ok = (!g_emit.flat_jmp_entry) || _pl_cells;  if (!g_emit.flat_pat && _gen_ok && _jmp_ok && x86_zc_frame() == ZC_FRAME_RSP && n > 0) { int _azd = 1; for (int _i = 0; _i < n; _i++) if (!zd_on[_i]) { if (_pl_cells && zd_out[_i] == -1) continue;    _azd = 0; break; } g_emit.flat_all_zd = _azd; } } { static int _lpd = -1; if (_lpd < 0) { const char * _e = getenv("SCRIP_LP_DIAG"); _lpd = (_e && *_e == '1') ? 1 : 0; } if (_lpd) { int _arm = 0; for (int _i = 0; _i < n; _i++) if (zd_on[_i]) _arm++; fprintf(stderr, "[LP] prefix=%s n=%d armed=%d all_zd=%d region=%d jmp=%d pat=%d gen=%d\n", prefix, n, _arm, g_emit.flat_all_zd, (g_emit_cfg ? g_emit_cfg->jcon_value_region : -1), g_emit.flat_jmp_entry, g_emit.flat_pat, g_emit.flat_gen); } } }       { extern void emit_fb_divergence_check(void); emit_fb_divergence_check(); }
    { static int _tp = -1; if (_tp < 0) { const char * e = getenv("SCRIP_TOP_PORTS"); _tp = (e && *e == '1') ? 1 : 0; }
      int _cls_o = (!bare && !g_emit.flat_jmp_entry && !g_emit.zframe_graph && !g_emit.flat_stmt_frame && !g_emit.flat_lcl_proc && !g_emit.flat_pat && !g_emit.flat_gen && !g_gen_proc_active && !(g_emit_cfg && (g_emit_cfg->icn_cells_graph || g_emit_cfg->pl_cells_graph)) && !(g_emit_cfg && g_emit_cfg->body_root) && g_emit.flat_outer_nparams == 0) ? 1 : 0;
      if (_cls_o) for (int _i = 0; _i < n; _i++) if (nodes[_i]->op == IR_SUSPEND) { _cls_o = 0; break; }
      _top_hoist = (_tp && _cls_o) ? 1 : 0;
      if (_top_hoist) {
          g_emit.flat_main_body_p = &lbl_α_body; g_emit.flat_main_ω_p = &lbl_ω;
          bb_emit_x86(bb_main_entry_bridge());
          emit_sep_rule('-'); emit_label_define_bb(&lbl_β); bb_emit_x86(bb_main_β());
          emit_sep_rule('-'); emit_label_define_bb(&lbl_γ); bb_emit_x86(bb_glue_outer_γ());
          emit_sep_rule('-'); emit_label_define_bb(&lbl_ω); bb_emit_x86(bb_glue_outer_ω());
          for (int _k = 1; _k <= 3; _k++) { int _has = 0; for (int _i = 0; _i < n; _i++) if (emit_floater_kind(nodes[_i]) == _k) { _has = 1; break; } if (!_has) continue; _flt_hoisted[_k] = 1;
              bb_label_t * _fa = emit_floater_label(_k); bb_label_t * _fg = emit_floater_label(1);
              g_emit.lbl_α_p = _fa; g_emit.lbl_α = _fa->name; g_emit.lbl_γ_p = _fg; g_emit.lbl_γ = _fg->name; g_emit.lbl_ω_p = &lbl_ω; g_emit.lbl_ω = lbl_ω.name; g_emit.lbl_β_p = _fa; g_emit.lbl_β = _fa->name;
              g_emit.op_ival = _k; g_emit.op_define_role = (_k == 3) ? 0 : _k;
              int _uid0 = g_flat_node_id; emit_sep_rule('-'); bb_emit_x86(bb_main_floater(_k)); _flt_uid_burn[_k] = g_flat_node_id - _uid0; g_flat_node_id = _uid0; }
          g_emit.flat_main_body_p = (bb_label_t *)0; g_emit.flat_main_ω_p = (bb_label_t *)0;
      } }
    if (text_externalise && g_is_text && !fn_face_dead) emit_label_define_bb(&lbl_α);
    else if (g_emit.zframe_graph || (g_emit_cfg && g_emit_cfg->icn_cells_graph && g_emit.flat_lcl_proc)) emit_label_define_bb(&lbl_α);
    if (g_is_text) g_emit_pos += 7;
    bb_label_t lbl_stcγ, lbl_stcω;
    if (g_emit.flat_stmt_frame) { emit_label_initf(&lbl_stcγ, "%s_stγ", fam); emit_label_initf(&lbl_stcω, "%s_stω", fam); }
    if (g_emit.zframe_graph) {
        { extern void xa_flat_zframe_prologue(void); xa_flat_zframe_prologue(); }
        if (g_emit_cfg && g_emit_cfg->pl_cells_graph && g_emit.flat_all_zd) { int _plk = 0; for (int _i = 0; _i < g_emit_cfg->n; _i++) { IR_t * _nd = g_emit_cfg->all[_i]; if (_nd && zd_wl_kind(_nd)) _plk += zd_k(_nd); } if (_plk > 0) { bb_emit_x86(x86("sub", "rsp", (long)_plk)); } }
    } else if (g_emit.flat_lcl_proc) {
        int kt2 = g_emit.flat_frame_bytes;
        int np = g_emit_cfg ? g_emit_cfg->nparams : 0;
        int nl = g_emit_cfg ? g_emit_cfg->nlocals : 0;
        int frame_total = kt2 + (np + nl) * 16;
        extern void rt_lcl_proc_args_install(void *, int, int);
        extern void rt_icn_zframe_args_install(void *, int, int);
        int _use_zframe_install = (g_emit_cfg && g_emit_cfg->icn_cells_graph) ? 1 : 0;
        std::string _lseed;
        { extern int zls_g_locals(const IR_graph_t *); extern int zls_g_region(const IR_graph_t *);
          static int _en = -1; if (_en < 0) { const char * _e = getenv("SCRIP_LCL_SEED"); _en = (_e && *_e == '0') ? 0 : 1; }
          int _lo = (_en && g_emit_cfg) ? zls_g_locals(g_emit_cfg) : -1;
          int _rg = (_en && g_emit_cfg) ? zls_g_region(g_emit_cfg) : -1;
          if (_lo >= 0 && _rg > _lo && _rg <= frame_total - 32)
              _lseed = x86("comment", "LCL-SEED: NULVCL the named-local vslot suffix [___+lo, ___+rg) so lexical locals read unbound, not stack residue")
                     + x86("mov", "rdi", "rsp") + x86("add", "rdi", (long)_lo) + x86("xor", "eax", "eax") + x86("mov32", "ecx", (long)(_rg - _lo)) + x86("rep_stosb"); }
        if (g_is_text) {
            char _lp[512]; int _lz = 0;
            _lz += snprintf(_lp + _lz, (int)sizeof(_lp) - _lz, "sub rsp, %d\nmov qword ptr [rsp + %d], rcx\nmov qword ptr [rsp + %d], rdx\n", frame_total, frame_total - 24, frame_total - 16);
            emit_text_n(_lp, strlen(_lp));
            if (!_lseed.empty()) bb_emit_x86(_lseed);
            _lz = 0; _lz += snprintf(_lp + _lz, (int)sizeof(_lp) - _lz, "mov rdi, rsp\nmov esi, %d\nmov edx, %d\ncall %s@PLT\n", np, nl, _use_zframe_install ? "rt_icn_zframe_args_install" : "rt_lcl_proc_args_install");
            emit_text_n(_lp, strlen(_lp));
        } else {
            if (frame_total <= 127) { ef_b3(0x48, 0x83, 0xEC); ef_b1((uint8_t)frame_total); } else { ef_b3(0x48, 0x81, 0xEC); bb_emit_u32((uint32_t)frame_total); }
            { int _d = frame_total - 24; ef_b2(0x48, 0x89); if (_d >= -128 && _d <= 127) { ef_b3(0x4C, 0x24, (uint8_t)(int8_t)_d); } else { ef_b2(0x8C, 0x24); bb_emit_u32((uint32_t)_d); } }
            { int _d = frame_total - 16; ef_b2(0x48, 0x89); if (_d >= -128 && _d <= 127) { ef_b3(0x54, 0x24, (uint8_t)(int8_t)_d); } else { ef_b2(0x94, 0x24); bb_emit_u32((uint32_t)_d); } }
            { static int _hd = -1; if (_hd < 0) { const char * _e = getenv("SCRIP_ICN_HDR_DEAD"); _hd = (_e && *_e == '1') ? 1 : 0; } if (_hd) { int _d = frame_total - 8; ef_b2(0x48, 0x89); if (_d >= -128 && _d <= 127) { ef_b3(0x6C, 0x24, (uint8_t)(int8_t)_d); } else { ef_b2(0xAC, 0x24); bb_emit_u32((uint32_t)_d); } } }   /* ⭐ N-1(a) s247: the [frame_total-8] "caller ____" slot is WRITE-ONLY — three sites fill it with two different registers (this one rbp, xa_flat:195/:247 rsp) and NOTHING READS IT: both zframe epilogues carry an empty string where the restore used to be, and neither arg installer touches the header (they write only [base+16*(i+1)]).  The store was BINARY-ONLY: the TEXT arm's snprintf passed frame_total-8 as a FOURTH vararg to a format with THREE conversions, so it was silently dropped — an m3 ≢ m4 divergence in the Icon prologue, inert only because the slot is dead.  Removed rather than added to TEXT: the media agree by losing an instruction, not by gaining a dead one.  SCRIP_ICN_HDR_DEAD=1 restores the store byte-exactly for A/B.  The slot disappears entirely at N-1(b/c) when the wire header gives way to the caller-pushed pair. */
            if (!_lseed.empty()) bb_emit_x86(_lseed);
            ef_b3(0x48, 0x89, 0xE7);
            ef_b1(0xBE); bb_emit_u32((uint32_t)np);
            ef_b1(0xBA); bb_emit_u32((uint32_t)nl);
            { uint64_t _fn = (uint64_t)(uintptr_t)(void *)(_use_zframe_install ? rt_icn_zframe_args_install : rt_lcl_proc_args_install); ef_b2(0x48, 0xB8); bb_emit_u64(_fn); ef_b2(0xFF, 0xD0); }
        }
    }
    if (!bare) emit_label_define_bb(&lbl_α_body);
    { extern std::string bb_zdp_origin(long); extern int x86_zdp_on_c(void); if (x86_zdp_on_c()) bb_emit_x86(bb_zdp_origin((long)0)); }   { if (x86_zdp_rbp_on()) bb_emit_x86(x86_zsm_ev(0)); }
    if (xa_flat_class_c_pred()) xa_flat_chain_prologue();
    { int _bfb = blob_frame_bytes(); if (_bfb > 0) bb_emit_x86(x86("comment", "R-4(b) BLOB ACTIVATION FRAME (THREE ZETAS): this stored-pattern blob is the callee of a *P DEFER and owns registry slots (ARBNO cell / capture SAVE / FENCE1 watermark) that must survive its own interior's jmp-entry crossings and be PER-ACTIVATION under recursion -- push rbp; mov rbp,rsp; carve. Whacked at ω (mov rsp,rbp; pop rbp), retained across γ with rbp restored to the caller's through the resume record.  WIRE-STACK ARM (s195): the caller PUSHed the pair before entry, so after push rbp;mov rbp,rsp it sits at [rbp+8]=γ [rbp+16]=ω -- law 0a's layout exactly -- and the head SOURCES the pair from there instead of from the caller-set registers; every downstream exit keeps reading the banked [rbp-8]/[rbp-16] unchanged.") + x86("push", "rbp") + x86("mov", "rbp", "rsp") + x86("sub", "rsp", (long)_bfb) + x86("mov", "rcx", RDQ("rbp", 8)) + x86("mov", RDQ("rbp", -8), "rcx") + x86("mov", "rcx", RDQ("rbp", 16)) + x86("mov", RDQ("rbp", -16), "rcx") + x86("mov", RDQ("rbp", -24), "rdx") + IF(sn4_blob_casmark(), x86("mov", RDQ("rbp", -32), "r12")));
      if (_bfb <= 0 && g_emit.flat_jmp_entry && g_emit.flat_pat) bb_emit_x86(x86("comment", "WIRE-STACK ARM (s195), FRAMELESS BLOB: no activation frame is carved here, so there is no [rbp+8]/[rbp+16] to read -- at entry the caller's pushed pair is at [rsp+0]=γ [rsp+8]=ω and is sourced from there; the pair stays on the stack under the interior's carve and the landing owns the release, so this arm depends on the LIFO unwind law (Lon s195: 'that will only work if RSP is guaranteed proper, and it should be based on LIFO unwind').") + x86("mov", "r10", RDQ("rsp", 0)) + x86("mov", "r11", RDQ("rsp", 8)));    }
    { extern int g_flat_outer_nparams; static int _gsym = -1; if (_gsym < 0) { const char * e = getenv("SCRIP_GLUE_SYM"); _gsym = (e && *e == '1') ? 1 : 0; } int _legacy = (!g_emit.flat_jmp_entry && !g_emit.flat_pat && !g_emit.flat_gen && !g_gen_proc_active && !g_emit.zframe_graph && !g_emit.flat_lcl_proc);  extern int g_glue_entered; g_glue_entered = (g_emit.flat_outer_nparams == 0 && _legacy) ? 1 : 0; if (g_glue_entered) bb_emit_x86(x86_main_prologue()); (void)_gsym; { static int _gluo = -1; if (_gluo < 0) { const char * e = getenv("SCRIP_GLUEO"); _gluo = (e && *e == '0') ? 0 : 1; } static int _gluod = -1; if (_gluod < 0) { const char * e = getenv("SCRIP_GLUEO_DIAG"); _gluod = (e && *e == '1') ? 1 : 0; } extern int g_glue_o_sup; g_glue_o_sup = (_gluo && g_glue_entered && !emit_rec_pin()) ? 1 : 0; if (g_glue_o_sup) g_glue_entered = 0; if (_gluod) fprintf(stderr, "[GLUEO] graph=%s entered=%d rec_pin=%d deep=%d pat=%d gen=%d -> closed_loop_suppressed=%d\n", g_emit.flat_lbl_α ? g_emit.flat_lbl_α : "<anon>", g_glue_entered, emit_rec_pin() ? 1 : 0, g_emit.flat_deep_arrival, g_emit.flat_pat, g_emit.flat_gen, g_glue_o_sup); } if (g_glue_entered) { { long _capN = 0; if (g_emit_cfg) for (int _ci = 0; _ci < g_emit_cfg->n; _ci++) { IR_t * _cs = g_emit_cfg->all[_ci]; if (_cs && _cs->op == IR_MATCH_ASSIGN_SAVE && cap_anchor_of(_cs) > 0) _capN++; } g_emit.op_fc_bytes = _capN > 0 ? (long)(64 + 16 * _capN) : 0; }    bb_emit_x86(bb_glue_framed_enter()); } }
    bb_label_t **lbls  = (bb_label_t **)alloca(sizeof(bb_label_t *) * n);
    bb_label_t **betas = (bb_label_t **)alloca(sizeof(bb_label_t *) * n);
    bb_label_t **ra_y  = (bb_label_t **)alloca(sizeof(bb_label_t *) * n);
    bb_label_t **ra_t  = (bb_label_t **)alloca(sizeof(bb_label_t *) * n);
    bb_label_t **na_s  = (bb_label_t **)alloca(sizeof(bb_label_t *) * n);
    bb_label_t ***fc_sig = (bb_label_t ***)alloca(sizeof(bb_label_t **) * n);
    bb_label_t **na_f  = (bb_label_t **)alloca(sizeof(bb_label_t *) * n);
    bb_label_t **st_pre = (bb_label_t **)alloca(sizeof(bb_label_t *) * n);
    int st_first_seen = 0;
    bb_label_t **st_x   = (bb_label_t **)alloca(sizeof(bb_label_t *) * n);
    for (int i = 0; i < n && g_flat_chain_set_n < FLAT_CHAIN_SET_MAX; i++) g_flat_chain_set[g_flat_chain_set_n++] = nodes[i];
    g_suspend_resume_slot = -1;
    if (g_gen_proc_active && g_emit_cfg && g_emit_cfg->resume_slot >= 0)
        for (int _si = 0; _si < n; _si++) if (nodes[_si]->op == IR_SUSPEND) { g_suspend_resume_slot = g_emit_cfg->resume_slot; break; }
    if (g_suspend_resume_slot < 0 && g_resumable_callable_active && g_emit_cfg && g_emit_cfg->resume_slot >= 0)
        g_suspend_resume_slot = g_emit_cfg->resume_slot;
    static int _diag_chain_seq = 0; int id = _diag_chain_seq++;
    int own_mark = -1;
    { extern int zls_off(const IR_t *); for (int _oi = 0; _oi < n; _oi++) if (nodes[_oi]->op == IR_MATCH_BEGIN) { int _ho = zls_off(nodes[_oi]); if (_ho >= 0) own_mark = _ho + 16; break; } }
    for (int i = 0; i < n; i++) {
        int _uid = g_flat_node_id++; const char * _kn = flat_label_kind(nodes[i]->op);
        lbls[i]  = emit_label_alloc("n%d_%s_α", _uid, _kn);
        { int _flk = emit_floater_kind(nodes[i]); if (_flk) lbls[i] = emit_floater_label(_flk); }
        if (g_emit_cfg && g_emit_cfg->n_balias > 0) { for (int _bi = 0; _bi < g_emit_cfg->n_balias; _bi++) if (g_emit_cfg->balias_node[_bi] == nodes[i]) { lbls[i] = emit_label_alloc("%s", g_emit_cfg->balias_name[_bi]); break; } }
        if (g_emit_cfg && g_emit_cfg->n_dentry > 0) { for (int _dq = 0; _dq < g_emit_cfg->n_dentry; _dq++) if (g_emit_cfg->dentry_entry[_dq] == nodes[i]) g_emit_cfg->dentry_name[_dq] = lbls[i]->name; }
        betas[i] = emit_label_alloc("n%d_%s_β", _uid, _kn);
        ra_y[i]  = (nodes[i]->op == IR_REPALT) ? emit_label_alloc("n%d_%s_ry", _uid, _kn) : NULL;
        ra_t[i]  = (nodes[i]->op == IR_REPALT) ? emit_label_alloc("n%d_%s_rt", _uid, _kn) : NULL;
        na_s[i]  = ((nodes[i]->op == IR_MATCH_ALTERNATE || nodes[i]->op == IR_MATCH_ARBNO || nodes[i]->op == IR_MATCH_FENCE1 || nodes[i]->op == IR_SCAN_SEQUENCE || nodes[i]->op == IR_SCAN_ALTERNATE || nodes[i]->op == IR_DISJUNCTION) && nodes[i]->n_operands > 0) ? emit_label_alloc("n%d_%s_as", _uid, _kn) : NULL;
        na_f[i]  = (((nodes[i]->op == IR_MATCH_ALTERNATE || nodes[i]->op == IR_MATCH_ARBNO || nodes[i]->op == IR_MATCH_FENCE1 || nodes[i]->op == IR_SCAN_SEQUENCE || nodes[i]->op == IR_SCAN_ALTERNATE || nodes[i]->op == IR_DISJUNCTION) && nodes[i]->n_operands > 0) || nodes[i]->op == IR_MATCH_BEGIN) ? emit_label_alloc("n%d_%s_af", _uid, _kn) : NULL;
        { extern const char * bb_src_of(const IR_t *); st_pre[i] = (g_emit.flat_stmt_frame && bb_src_of(nodes[i])) ? emit_label_alloc("n%d_%s_st", _uid, _kn) : NULL; }
        st_x[i] = (st_pre[i] && st_first_seen) ? emit_label_alloc("n%d_%s_sx", _uid, _kn) : NULL; if (st_pre[i]) st_first_seen = 1;
        fc_sig[i] = NULL;
        if (nodes[i]->op == IR_MATCH_ALTERNATE && nodes[i]->n_operands > 0) {
            int _N = (int)(nodes[i]->n_operands / 2);
            fc_sig[i] = (bb_label_t **)alloca(sizeof(bb_label_t *) * _N);
            for (int _j = 0; _j < _N; _j++) fc_sig[i][_j] = emit_label_alloc("n%d_%s_s%d", _uid, _kn, _j);
        }
    }
    unsigned char *bused = (unsigned char *)alloca(n > 0 ? n : 1);
    flat_beta_used_scan(nodes, n, bused);
    { static int _uwl = -1; if (_uwl < 0) { const char *_e = getenv("SCRIP_UNWIND"); _uwl = (_e && *_e == '0') ? 0 : 1; }
      if (_uwl) for (int _i = 0; _i < n; _i++) {
          int _hi = _i;
          if (!zd_on[_i] || zd_wp[_i] <= 0) continue;
          if (nodes[_i]->op == IR_STATEMENT || nodes[_i]->op == IR_STATEMENT_BEGIN || nodes[_i]->op == IR_STATEMENT_END) continue;
          { int _bg = 0; for (int _j = _i - 1; _j >= 0; _j--) { int _o = (int)nodes[_j]->op; if (_o == IR_STATEMENT_BEGIN) { _bg = 1; break; } if (_o == IR_STATEMENT_END || _o == IR_STATEMENT) break; } if (!_bg) continue; }
          { IR_t *_ti = zd_chase(nodes[_i]->ω.node); if (!_ti) continue;
            for (int _j = _i - 1; _j >= 0; _j--) {
                int _o = (int)nodes[_j]->op; if (_o == IR_STATEMENT_BEGIN || _o == IR_STATEMENT_END || _o == IR_STATEMENT) break;
                if (zd_on[_j] && zd_wp[_j] > 0 && flat_unwind_beta(nodes[_j]) && zd_chase(nodes[_j]->ω.node) == _ti) {
                    if ((zd_wp[_i] + zd_k(nodes[_i])) - (zd_wp[_j] + zd_k(nodes[_j])) >= 0) bused[_j] = 1; break; } } } } }
    bb_label_t *resume_init_lbl = NULL;
    if (g_suspend_resume_slot >= 0 && g_gen_proc_active) {
        for (int _si = 0; _si < n; _si++) if (nodes[_si]->op == IR_SUSPEND) { resume_init_lbl = betas[_si]; break; }
    }
    if (resume_init_lbl) {
        if (g_is_text) {
            char _init[256];
            snprintf(_init, sizeof _init,
                "lea rax, [rip + %s]\nmov qword ptr [%s + %d], rax\n",
                resume_init_lbl->name, emit_rec_rsp_arm() ? "rsp" : emit_rec_fb(), g_suspend_resume_slot);
            emit_text_n(_init, strlen(_init));
        } else {
            ef_b3(0x48, 0x8D, 0x05); bb_emit_patch_rel32(resume_init_lbl);
            { int z = emit_rec_rsp_arm() ? 4 : emit_rec_fb_num(), lo = z & 7; ef_b2((uint8_t)(0x48 | (z >= 8 ? 0x01 : 0x00)), 0x89); if (lo == 4) ef_b2(0x84, 0x24); else ef_b1((uint8_t)(0x80 | lo)); bb_emit_u32((uint32_t)(unsigned)g_suspend_resume_slot); }
        }
    }
    for (int _li = 0; _li < n; _li++) if (nodes[_li]->op == IR_LIMIT) {
        g_emit.op_off = drive_value_slot(nodes[_li]);
        bb_emit_x86(bb_limit_init());
    }
    for (int i = 0; i < n; i++) {
        { int _fk = emit_floater_kind(nodes[i]); if (_fk && _flt_hoisted[_fk]) { g_flat_node_id += _flt_uid_burn[_fk]; continue; } }
        if (g_emit.flat_stmt_frame && st_pre[i]) { g_emit.xa_bb_emit_pair_n = 2; g_emit.xa_bb_emit_pair_define[0] = st_x[i]; g_emit.xa_bb_emit_pair_jmp[0] = NULL; g_emit.xa_bb_emit_pair_define[1] = st_pre[i]; g_emit.xa_bb_emit_pair_jmp[1] = NULL; g_emit.op_fc_bytes = 0; bb_emit_x86((st_x[i] ? x86_deflabel_pair(0) + bb_glue_framed_leave() : std::string()) + x86_deflabel_pair(1) + bb_glue_framed_enter()); }
        emit_zeta_selfload();
        { static int _beo = -1; if (_beo < 0) { const char *_e = getenv("SCRIP_BETA_ELIDE_OFF"); _beo = (_e && _e[0] == '1') ? 1 : 0; } g_emit.op_beta_dead = (_beo || bused[i]) ? 0 : 1; }
        g_emit.op_wpop = 0;
        { extern int zls_off(const IR_t *); extern int zls_result_off(const IR_t *); extern int zls_node_bytes(const IR_t *); int _zo = zls_off(nodes[i]); int _zr = zls_result_off(nodes[i]);
          g_emit.op_own_mark = own_mark; g_emit.op_own_ci = (_zo >= 0) ? ((_zr >= 0 ? _zr : _zo) + zls_node_bytes(nodes[i])) : 0; }
        bb_label_t *node_γ = &lbl_γ;
        bb_label_t *node_ω = &lbl_ω;
        IR_t *gtgt = nodes[i]->γ.node;
        IR_t *otgt = nodes[i]->ω.node;
        int gamma_is_beta = port_sz_beta(nodes[i]->γ.sz);
        int omega_is_beta = port_sz_beta(nodes[i]->ω.sz);
        int gamma_is_sig  = (nodes[i]->γ.sz[0] == (char)0xcf && (unsigned char)nodes[i]->γ.sz[1] == 0x83);
        int gamma_is_phi  = (nodes[i]->γ.sz[0] == (char)0xcf && (unsigned char)nodes[i]->γ.sz[1] == 0x86);
        int omega_is_phi  = (nodes[i]->ω.sz[0] == (char)0xcf && (unsigned char)nodes[i]->ω.sz[1] == 0x86);
        { int _gg = 0; IR_t * _g = gtgt; while (_g && _g->op == IR_GOTO && _gg++ < 128) { if (!gamma_is_beta) gamma_is_beta = port_sz_beta(_g->γ.sz); if (!gamma_is_sig) gamma_is_sig = (_g->γ.sz[0] == (char)0xcf && (unsigned char)_g->γ.sz[1] == 0x83); if (!gamma_is_phi) gamma_is_phi = (_g->γ.sz[0] == (char)0xcf && (unsigned char)_g->γ.sz[1] == 0x86); gtgt = _g->γ.node; _g = gtgt; } }
        { int _gg = 0; IR_t * _o = otgt; while (_o && _o->op == IR_GOTO && _gg++ < 128) { if (!omega_is_beta) omega_is_beta = port_sz_beta(_o->γ.sz); if (!omega_is_phi) omega_is_phi = (_o->γ.sz[0] == (char)0xcf && (unsigned char)_o->γ.sz[1] == 0x86); otgt = _o->γ.node; _o = otgt; } }
        int omega_is_retry = omega_is_beta && !beta_is_stmt_land(otgt);
        for (int k = 0; k < n; k++) if (nodes[k] == gtgt) {
            node_γ = (gamma_is_phi && na_f[k]) ? na_f[k] : (gamma_is_sig && na_s[k]) ? na_s[k] : gamma_is_beta ? betas[k] : lbls[k];
            if (gamma_is_sig && fc_sig[k]) {
                int _N = (int)(nodes[k]->n_operands / 2), _arm = 0, _src = -1, _best = -1;
                for (int _p = 0; _p < n; _p++) if (nodes[_p] == nodes[i]) { _src = _p; break; }
                for (int _j = 0; _j < _N; _j++) { IR_t *_e = nodes[k]->operands[2 * _j]; for (int _p = 0; _p < n; _p++) if (nodes[_p] == _e) { if (_p <= _src && _p > _best) { _best = _p; _arm = _j; } break; } }
                node_γ = fc_sig[k][_arm];
            }
            if (gamma_is_sig) { node_γ = node_γ; }
            if (gamma_is_phi) { node_γ = node_γ; }
            break;
        }
        { int _flk = emit_floater_kind(gtgt); if (_flk && node_γ == &lbl_γ) node_γ = emit_floater_label(_flk); }
        { if (gtgt && gtgt->op == IR_CALL && emit_floater_member(gtgt) && node_γ == &lbl_γ) node_γ = emit_floater_label(1); }
        if (nodes[i]->γ.node == NULL || nodes[i]->γ.node->op == IR_SUCCEED) node_γ = &lbl_γ;
        if (nodes[i]->γ.node && nodes[i]->γ.node->op == IR_FAIL) node_γ = &lbl_ω;
        int omega_resolved = 0;
        for (int k = 0; k < n; k++) if (nodes[k] == otgt) { node_ω = (omega_is_phi && na_f[k]) ? na_f[k] : omega_is_beta ? betas[k] : lbls[k]; omega_resolved = 1; break; }
        if (!omega_resolved) { int _flk = emit_floater_kind(otgt); if (_flk) { node_ω = emit_floater_label(_flk); omega_resolved = 1; } }
        if (!omega_resolved) node_ω = (otgt && otgt->op == IR_SUCCEED) ? &lbl_γ : &lbl_ω;
        g_emit.op_omega_is_death = (!omega_resolved && !(otgt && otgt->op == IR_SUCCEED)) ? 1 : 0;
        if (getenv("SCRIP_OMEGA_DIAG")) fprintf(stderr, "[OMEGA-DIAG] i=%d node_op=%s omega_resolved=%d otgt_op=%s op_omega_is_death=%d\n", i, bb_op_name(nodes[i]->op), omega_resolved, otgt ? bb_op_name(otgt->op) : "NULL", g_emit.op_omega_is_death);
        for (int r = 0; r < n; r++) if (nodes[r]->op == IR_REPALT && nodes[r]->n_operands > 0 && nodes[r]->operands[0] == nodes[i]) { node_γ = ra_y[r]; node_ω = ra_t[r]; break; }
        if (nodes[i]->op == IR_CONJUNCTION && nodes[i]->n_operands > 0 && nodes[i]->operands[0]) {
            IR_t * op0 = nodes[i]->operands[0];
            { int _s0 = nd_slot(op0); if (_s0 >= 0 && bb_slot_get(nodes[i]) < 0) bb_slot_register(nodes[i], _s0); }
        }
        g_limit_gen_beta = NULL;
        g_scan_body_beta = NULL;
        g_create_body_entry = NULL;
        g_move_label_tgt = NULL;
        g_suspend_dobody_beta = NULL;
        if (nodes[i]->op == IR_SUSPEND && nodes[i]->n_operands > 1 && nodes[i]->operands[1]) {
            IR_t *dobody = nodes[i]->operands[1];
            if (dobody->op == IR_FAIL) { g_suspend_dobody_beta = &lbl_ω; }
            else { for (int k = 0; k < n; k++) if (nodes[k] == dobody) { g_suspend_dobody_beta = (ir_is_generator_kind(dobody->op) || dobody->op == IR_CALL || dobody->op == IR_CALL_PROC_STAGED) ? betas[k] : lbls[k]; break; } }
        }
        if (nodes[i]->op == IR_CREATE && nodes[i]->n_operands > 0 && nodes[i]->operands[0]) {
            IR_t *body_entry = nodes[i]->operands[0];
            g_emit.op_sval_lbl = NULL;
            g_create_body_entry = NULL;
            for (int k = 0; k < n; k++) if (nodes[k] == body_entry) { g_emit.op_sval_lbl = lbls[k]->name; g_create_body_entry = lbls[k]; break; }
        }
        if (nodes[i]->op == IR_MOVE_LABEL && nodes[i]->n_operands > 0 && nodes[i]->operands[0]) {
            IR_t *rtgt = nodes[i]->operands[0];
            int wantb = (int) IR_LIT(nodes[i]).ival;
            if (rtgt->op == IR_FAIL) g_move_label_tgt = &lbl_ω;
            else if (rtgt->op == IR_SUCCEED) g_move_label_tgt = &lbl_γ;
            else for (int k = 0; k < n; k++) if (nodes[k] == rtgt) { g_move_label_tgt = wantb ? betas[k] : lbls[k]; if (wantb && g_emit_cfg && g_emit_cfg->pl_cells_graph) { g_emit.lbl_t1 = lbls[k]->name; g_emit.lbl_t1_p = lbls[k]; }    break; }
        }
        if (nodes[i]->op == IR_MOVE_LABEL && nodes[i]->n_operands > 1 && nodes[i]->operands[1]) {
            IR_t *igp = nodes[i]->operands[1];
            IR_t *sg = igp->γ.node;
            int sg_is_beta = port_sz_beta(igp->γ.sz);
            if (sg == NULL || sg->op == IR_SUCCEED) node_γ = &lbl_γ;
            else if (sg->op == IR_FAIL) node_γ = &lbl_ω;
            else for (int k = 0; k < n; k++) if (nodes[k] == sg) { node_γ = sg_is_beta ? betas[k] : lbls[k]; break; }
        }
        if (nodes[i]->op == IR_REPALT) {
            { static int _dd = -1; if (_dd < 0) { const char *_e = getenv("SCRIP_DRIVE_DIAG"); _dd = (_e && _e[0] == '1') ? 1 : 0; }
              long _p0 = _dd ? emit_text_count() : 0;
              flat_drive_repalt(nodes, n, i, lbls, betas, ra_y, ra_t, node_γ, node_ω);
              if (_dd && emit_text_count() == _p0) fprintf(stderr, "[DRIVE-DIAG] ZERO-EMIT chain=%d i=%d op=%s n_operands=%d (repalt path)\n", id, i, bb_op_name(nodes[i]->op), nodes[i]->n_operands); }
            continue;
        }
        if ((nodes[i]->op == IR_MATCH_ALTERNATE || nodes[i]->op == IR_MATCH_ARBNO || nodes[i]->op == IR_MATCH_FENCE1 || nodes[i]->op == IR_SCAN_SEQUENCE || nodes[i]->op == IR_SCAN_ALTERNATE || nodes[i]->op == IR_DISJUNCTION) && nodes[i]->n_operands > 0) {
            { static int _dd = -1; if (_dd < 0) { const char *_e = getenv("SCRIP_DRIVE_DIAG"); _dd = (_e && _e[0] == '1') ? 1 : 0; }
              long _p0 = _dd ? emit_text_count() : 0;
              { flat_drive_match_alt(nodes, n, i, lbls, betas, na_s, na_f, fc_sig, node_γ, node_ω, &lbl_ω); }
              if (_dd && emit_text_count() == _p0) fprintf(stderr, "[DRIVE-DIAG] ZERO-EMIT chain=%d i=%d op=%s n_operands=%d (match_alt path)\n", id, i, bb_op_name(nodes[i]->op), nodes[i]->n_operands); }
            continue;
        }
        if (nodes[i]->op == IR_LIMIT) {
            IR_t *gen = nodes[i]->n_operands > 0 ? nodes[i]->operands[0] : NULL;
            if (gen) for (int k = 0; k < n; k++) if (nodes[k] == gen) { g_limit_gen_beta = betas[k]; break; }
        }
        if (nodes[i]->op == IR_SCAN) {
            IR_t *bv = nodes[i]->n_operands > 1 ? nodes[i]->operands[1] : NULL;
            g_scan_body_beta = NULL;
            if (bv && ir_is_generator_kind(bv->op)) for (int k = 0; k < n; k++) if (nodes[k] == bv) { g_scan_body_beta = betas[k]; break; }
        }
        if (nodes[i]->op == IR_GALT && nodes[i]->n_operands >= 2) {
            IR_t *arm2 = nodes[i]->operands[0]; IR_t *arm1 = nodes[i]->operands[1];
            g_emit.lbl_t0_p = NULL; g_emit.lbl_t0 = NULL;
            g_emit.lbl_t1_p = NULL; g_emit.lbl_t1 = NULL;
            if (arm1) for (int k = 0; k < n; k++) if (nodes[k] == arm1) { g_emit.lbl_t0_p = lbls[k]; g_emit.lbl_t0 = lbls[k] ? lbls[k]->name : NULL; break; }
            if (arm2) for (int k = 0; k < n; k++) if (nodes[k] == arm2) { g_emit.lbl_t1_p = lbls[k]; g_emit.lbl_t1 = lbls[k] ? lbls[k]->name : NULL; break; }
        }
        { static int _zpf = -1; if (_zpf < 0) { const char *_e = getenv("SCRIP_ZPOP_FOLD_OFF"); _zpf = (_e && _e[0] == '1') ? 1 : 0; }
          if (!_zpf && x86_zc_frame() == ZC_FRAME_RSP && (x86_port_mode() == ZC_PORT_FORTH || x86_port_mode() == ZC_PORT_HEAP)) {
              long _sum = 0; int _fk = -1, _hops = 0;
              for (int _k = 0; _k < n; _k++) if (node_ω == betas[_k]) { _fk = _k; break; }
              if (_fk >= 0 && beta_is_stmt_land(nodes[_fk])) { _fk = -1; }
              while (_fk >= 0 && _hops++ < 64 && flat_trivial_beta(nodes[_fk])) {
                  { long _fck = 0; if (fc_geom(nodes[_fk], &_fck)) _sum += _fck; }
                  IR_t *_c = nodes[_fk]; IR_t *_o = _c->ω.node;
                  int _ib = port_sz_beta(_c->ω.sz);
                  int _ip = (_c->ω.sz[0] == (char)0xcf && (unsigned char)_c->ω.sz[1] == 0x86);
                  { int _gg = 0; IR_t *_g = _o; while (_g && _g->op == IR_GOTO && _gg++ < 128) { if (!_ib) _ib = port_sz_beta(_g->γ.sz); if (!_ip) _ip = (_g->γ.sz[0] == (char)0xcf && (unsigned char)_g->γ.sz[1] == 0x86); _o = _g->γ.node; _g = _o; } }
                  bb_label_t *_next = NULL; int _k2 = -1;
                  for (int _k = 0; _k < n; _k++) if (nodes[_k] == _o) { _k2 = _k; break; }
                  if (_k2 >= 0) { _next = (_ip && na_f[_k2]) ? na_f[_k2] : _ib ? betas[_k2] : lbls[_k2]; }
                  else _next = (_o && _o->op == IR_SUCCEED) ? &lbl_γ : &lbl_ω;
                  for (int _r = 0; _r < n; _r++) if (nodes[_r]->op == IR_REPALT && nodes[_r]->n_operands > 0 && nodes[_r]->operands[0] == _c) { _next = ra_t[_r]; break; }
                  node_ω = _next;
                  _fk = -1; for (int _k = 0; _k < n; _k++) if (_next == betas[_k]) { _fk = _k; break; }
              }
              g_emit.op_wpop = (int)_sum;
          } }
        { static int _dd = -1; if (_dd < 0) { const char *_e = getenv("SCRIP_DRIVE_DIAG"); _dd = (_e && _e[0] == '1') ? 1 : 0; }
          long _p0 = _dd ? emit_text_count() : 0;
          g_emit.op_scan = 0; g_emit.op_scan_head_off = -1;
          if (g_emit.flat_stmt_frame) {
              for (int _k = 0; _k < n; _k++) { bb_label_t * _st = st_x[_k] ? st_x[_k] : st_pre[_k]; if (_st && node_γ == lbls[_k]) node_γ = _st; if (_st && node_ω == lbls[_k]) node_ω = _st; }
              if (node_γ == &lbl_γ) node_γ = &lbl_stcγ; if (node_γ == &lbl_ω) node_γ = &lbl_stcω;
              if (node_ω == &lbl_ω) node_ω = &lbl_stcω; if (node_ω == &lbl_γ) node_ω = &lbl_stcγ; }
          if (nodes[i]->op == IR_MATCH_DEFER) {
              int _hk = -1; for (int _k = 0; _k < n; _k++) if (nodes[_k]->op == IR_MATCH_BEGIN) { _hk = _k; break; }
              if (_hk >= 0) { IR_t *_gt = nodes[_hk]->γ.node; { int _gg = 0; while (_gt && _gt->op == IR_GOTO && _gg++ < 128) _gt = _gt->γ.node; }
                  if (_gt == nodes[i] && node_ω == betas[_hk]) { g_emit.op_scan = 1; g_emit.op_scan_head_off = (int)drive_value_slot(nodes[_hk]); } }
          }
          int _uw_stolen = 0; long _uw_pop = 0;
          { static int _uw = -1; if (_uw < 0) { const char *_e = getenv("SCRIP_UNWIND"); _uw = (_e && *_e == '0') ? 0 : 1; }
            int _h3 = i;
            if (_uw && nodes[i]->op != IR_STATEMENT && nodes[i]->op != IR_STATEMENT_BEGIN && nodes[i]->op != IR_STATEMENT_END && zd_on[i] && zd_wp[i] > 0 && g_emit.op_wpop == 0 && !omega_is_retry && !omega_is_phi && !g_emit.flat_stmt_frame) {
                int _bg = 0; for (int _j = i - 1; _j >= 0; _j--) { int _o = (int)nodes[_j]->op; if (_o == IR_STATEMENT_BEGIN) { _bg = 1; break; } if (_o == IR_STATEMENT_END || _o == IR_STATEMENT) break; }
                IR_t *_ti = zd_chase(nodes[i]->ω.node);
                if (_bg && _ti) for (int _j = i - 1; _j >= 0; _j--) {
                    int _o = (int)nodes[_j]->op; if (_o == IR_STATEMENT_BEGIN || _o == IR_STATEMENT_END || _o == IR_STATEMENT) break;
                    if (zd_on[_j] && zd_wp[_j] > 0 && flat_unwind_beta(nodes[_j]) && zd_chase(nodes[_j]->ω.node) == _ti) {
                        long _d = (zd_wp[i] + zd_k(nodes[i])) - (zd_wp[_j] + zd_k(nodes[_j]));
                        if (_d >= 0) { node_ω = betas[_j]; _uw_stolen = 1; _uw_pop = _d; } break; } } } }
          int _endj_stolen = 0;
          { static int _ej = -1; if (_ej < 0) { const char *_e = getenv("SCRIP_ZD_ENDJMP"); _ej = (_e && *_e == '0') ? 0 : 1; }
            int _h2 = i;
            if (_ej && !_uw_stolen && nodes[i]->op != IR_STATEMENT && nodes[i]->op != IR_STATEMENT_BEGIN && nodes[i]->op != IR_STATEMENT_END && zd_on[i] && zd_wp[i] > 0 && g_emit.op_wpop == 0 && !omega_is_retry && !omega_is_phi && !g_emit.flat_stmt_frame) {
                int _e2 = -1; for (int _j = i + 1; _j < n; _j++) { if (nodes[_j]->op == IR_STATEMENT_BEGIN) break; if (nodes[_j]->op == IR_STATEMENT_END || nodes[_j]->op == IR_STATEMENT) { _e2 = _j; break; } }
                if (_e2 >= 0 && zd_gp[_e2] == zd_wp[i] + zd_k(nodes[i])) {
                    IR_t *_tf = zd_chase(nodes[i]->ω.node); IR_t *_te = zd_chase(nodes[_e2]->γ.node);
                    int _kf = -1; if (_tf) for (int _k = 0; _k < n; _k++) if (nodes[_k] == _tf) { _kf = _k; break; }
                    if (_tf && _tf == _te && _kf >= 0 && node_ω == lbls[_kf]) { node_ω = lbls[_e2]; _endj_stolen = 1; g_emit.op_wpop = 0; } } } }
          bb_label_t *_zw5_saved_omega = NULL; int _zw5_wpop_stolen = 0;
          { int _h = i;
          if (_zw5_on && !_endj_stolen && !_uw_stolen && nodes[i]->op != IR_STATEMENT && nodes[i]->op != IR_STATEMENT_END && zd_on[i] && zd_wp[i] > 0 && !omega_is_retry && !omega_is_phi) { for (int _si = 0; _si < zw5_pool_stmts; _si++) { int _si_base = 0; for (int _sj = 0; _sj < _si; _sj++) _si_base += zw5_stmt_cnt[_sj]; if (zw5_stmt_idx[_si] > i) { for (int _d = 0; _d < zw5_stmt_cnt[_si]; _d++) { if (zw5_stmt_depths[_si][_d] == zd_wp[i]) { _zw5_saved_omega = node_ω; node_ω = &zw5_pool[_si_base + _d]; _zw5_wpop_stolen = zd_wp[i]; break; } } break; } } } }
          if (zd_on[i] || zd_gp[i] > 0 || zd_wp[i] > 0) { g_zd_stage = 1; g_zd_arm = (zd_on[i] && !(g_emit_cfg && g_emit_cfg->pl_cells_graph && !g_emit.flat_all_zd)) ? 1 : 0; int _pl_suppress = (g_emit_cfg && g_emit_cfg->pl_cells_graph && !g_emit.flat_all_zd) ? 1 : 0; g_zd_gpop = _pl_suppress ? 0 : zd_gp[i]; g_zd_wpop = _pl_suppress ? 0 : (_uw_stolen ? (int)_uw_pop : ((_zw5_wpop_stolen || _endj_stolen) ? 0 : zd_wp[i])); g_zd_wsteal = _endj_stolen || _uw_stolen;
              g_zd_k = zd_on[i] ? zd_k(nodes[i]) : 0;
              { g_zd_zunder = 0; if (zd_on[i] && nodes[i]->op == IR_MATCH_REPLACE) { int _zu = 0; for (int _zj = i - 1; _zj >= 0; _zj--) { if (nodes[_zj]->op == IR_MATCH_END) break; if (zd_on[_zj]) _zu += zd_k(nodes[_zj]); } g_zd_zunder = _zu; int _zp = 0, _inpat = 0; for (int _zj = i - 1; _zj >= 0; _zj--) { if (nodes[_zj]->op == IR_MATCH_BEGIN) break; if (nodes[_zj]->op == IR_MATCH_END) { _inpat = 1; continue; } if (_inpat && zd_on[_zj] && nodes[_zj]->op >= IR_MATCH && nodes[_zj]->op <= IR_MATCH_VALUE) _zp += zd_k(nodes[_zj]) - fence0_release_bytes(nodes[_zj]);    } g_zd_zpat = _zp; { extern int fc_head_fp(const IR_t *); IR_t * _mb = (IR_t *)0; int _jh = -1; for (int _zj = i - 1; _zj >= 0; _zj--) { if (nodes[_zj]->op == IR_MATCH_BEGIN) { _mb = nodes[_zj]; _jh = _zj; break; } } int _fp = _mb ? fc_head_fp(_mb) : -1; g_zd_zfc = (_fp >= 0) ? _fp : 0; (void)_jh; } { static int _zpd = -1; if (_zpd < 0) { const char * _e = getenv("SCRIP_ZPAT_DIAG"); _zpd = (_e && *_e == '1') ? 1 : 0; } if (_zpd) fprintf(stderr, "[ZPAT] i=%d zunder=%d zpat=%d zfc=%d zout_repl=%d op=%s\n", i, g_zd_zunder, g_zd_zpat, g_zd_zfc, zd_out[i], bb_op_name(nodes[i]->op)); } } }       { g_zd_ztail = 0; if (zd_on[i] && (nodes[i]->op == IR_TO || nodes[i]->op == IR_TO_BY)) { int _zttail = 0; for (int _zt = i + 1; _zt < n; _zt++) { if (zd_on[_zt]) _zttail += zd_k(nodes[_zt]); } g_zd_ztail = _zttail; } }
              for (int _zj = 0; _zj < 6; _zj++) { g_zd_read[_zj] = 0; g_zd_kind[_zj] = -1; }
              if (zd_on[i]) { int _no = zd_nops(nodes[i]);
                  int _op = (int)nodes[i]->op; int _cap = (_op == IR_MATCH_ASSIGN_COND || _op == IR_MATCH_ASSIGN_IMM);
                  for (int _zj = 0; _zj < _no && _zj < 6; _zj++) { if (_cap && _zj == 0) continue;
                  IR_t * _p = nodes[i]->operands[_zj]; for (int _k = 0; _k < n; _k++) if (nodes[_k] == _p) { int _xh = 0; for (int _zm = i - 1; _zm > _k; _zm--) { if (zd_arm[i] >= 0 && _zm == zd_arm[i]) { _xh += 32; continue; }    if (nodes[_zm]->op == IR_MATCH_DEFER) { _xh += 16; continue; }    if (nodes[_zm]->op != IR_MATCH_BEGIN) continue; extern int fc_head_fp(const IR_t *); extern int fc_tail_head(const IR_t *); extern int emit_match_rbp(void); if (emit_match_rbp()) { extern int sn4_xh_frame_extra(void); extern int emit_match_begin_frame_extra(const IR_t *); _xh += 64 + (sn4_xh_frame_extra() ? emit_match_begin_frame_extra(nodes[_zm]) : 0); }    else if (x86_zc_frame() == ZC_FRAME_RSP && (x86_port_mode() == ZC_PORT_FORTH || x86_port_mode() == ZC_PORT_HEAP) && (fc_head_fp(nodes[_zm]) >= 0 || fc_tail_head(nodes[_zm]))) _xh += 32; } g_zd_read[_zj] = zd_out[i] - zd_out[_k] + _xh;       g_zd_kind[_zj] = (int)ir_norm_call_kind(_p->op); break; } } } }
              { if (zd_on[i] && nodes[i]->op == IR_MATCH_REPLACE) { g_zd_read[2] = -1; g_zd_kind[2] = -1; IR_t * _mb2 = (IR_t *)0; int _jh2 = -1; for (int _zj = i - 1; _zj >= 0; _zj--) { if (nodes[_zj]->op == IR_MATCH_BEGIN) { _mb2 = nodes[_zj]; _jh2 = _zj; break; } } IR_t * _sp = (_mb2 && _mb2->n_operands > 0) ? _mb2->operands[0] : (IR_t *)0; if (_sp && _jh2 >= 0) { for (int _js = _jh2 - 1; _js >= 0; _js--) if (nodes[_js] == _sp) { if (zd_on[_js]) { g_zd_read[2] = g_zd_zunder + (zd_out[_jh2] - zd_out[_js]); g_zd_kind[2] = (int)_sp->op; } if (getenv("SCRIP_ZPAT_DIAG")) fprintf(stderr, "[SUBJ2] jh=%d js=%d zout_jh=%d zout_js=%d zon_js=%d staged=%d\n", _jh2, _js, zd_out[_jh2], zd_out[_js], zd_on[_js], g_zd_read[2]); break; } } } }
          if (nodes[i]->op == IR_MATCH_BEGIN) { g_emit.lbl_t0 = na_f[i] ? na_f[i]->name : NULL; g_emit.lbl_t0_p = na_f[i]; }
          if (nodes[i]->op == IR_SUSPEND && g_emit.zframe_graph && g_emit_cfg && g_emit_cfg->icn_zframe_gen && g_suspend_dobody_beta) { g_zd_suspend_uclaim = 0; g_zd_stage = 1; }
          emit_drive(nodes[i], lbls[i], node_γ, node_ω, betas[i]);
          if (_zw5_saved_omega) node_ω = _zw5_saved_omega;
          if (_zw5_on && nodes[i]->op == IR_STATEMENT) { for (int _si = 0; _si < zw5_pool_stmts; _si++) { if (zw5_stmt_idx[_si] != i) continue; int _si_base = 0; for (int _sj = 0; _sj < _si; _sj++) _si_base += zw5_stmt_cnt[_sj]; for (int _d = 0; _d < zw5_stmt_cnt[_si]; _d++) { emit_label_define_bb(&zw5_pool[_si_base + _d]); bb_emit_x86(x86("add", "rsp", (long)zw5_stmt_depths[_si][_d]) + x86("jmp", "extlbl", (uint64_t)(uintptr_t)node_ω)); } break; } }
          if (_dd && emit_text_count() == _p0) fprintf(stderr, "[DRIVE-DIAG] ZERO-EMIT chain=%d i=%d op=%s n_operands=%d\n", id, i, bb_op_name(nodes[i]->op), nodes[i]->n_operands); }
    }
    g_emit.op_beta_dead = 0;
    g_emit.op_wpop = 0;
    if (n == 0) emit_jmp_label(flat_empty_body_fail ? &lbl_ω : &lbl_γ, JMP_JMP);
    if (g_emit.flat_jmp_entry) {
        if (!bare) {
        emit_sep_rule('-'); emit_label_define_bb(&lbl_res);
        if (g_emit.flat_pat) {
            bb_emit_x86(IF(blob_frame_bytes() > 0, x86_rsp_load64("rbp", 24)) + x86("add", "rsp", 32L));
        } else {
        if (g_is_text) {
            char _res[96];
            snprintf(_res, sizeof _res, "add rsp, 8\npop %s\n", emit_rec_fb());
            emit_text_n(_res, strlen(_res));
        } else {
            int z = emit_rec_fb_num();
            ef_b4(0x48, 0x83, 0xC4, 0x08);
            if (z >= 8) ef_b2(0x41, (uint8_t)(0x58 | (z & 7))); else ef_b1((uint8_t)(0x58 | (z & 7)));
        }
        }
        }
    }
    if (!bare && !_top_hoist) {
    emit_sep_rule('-'); emit_label_define_bb(&lbl_β);
    {
        bb_label_t *resume_tgt = &lbl_ω;
        for (int i = 0; i < n; i++) if (nodes[i]->op == IR_SUSPEND) { resume_tgt = betas[i]; break; }
        if (g_emit_cfg && g_emit_cfg->body_root)
            for (int i = 0; i < n; i++) if (nodes[i] == g_emit_cfg->body_root && nodes[i]->op == IR_DISJUNCTION) { resume_tgt = lbls[i]; break; }
        if (g_emit_cfg && g_emit_cfg->body_root)
            for (int i = 0; i < n; i++) if (nodes[i] == g_emit_cfg->body_root && nodes[i]->op == IR_CALL_BUILTIN_GEN) { resume_tgt = betas[i]; break; }
        { extern int sn4_defer_resume(void); const int _dres = sn4_defer_resume();
          if (getenv("SCRIP_RESUME_WHY")) fprintf(stderr, "[RESUME-GATE] dres=%d flat_pat=%d cfg=%d body_root=%p\n", _dres, g_emit.flat_pat, g_emit_cfg ? 1 : 0, (void *)(g_emit_cfg ? g_emit_cfg->body_root : (IR_t *)0));
          if (_dres && g_emit.flat_pat && g_emit_cfg && g_emit_cfg->body_root) {
            int _f1 = 0; for (int i = 0; i < n; i++) if (nodes[i]->op == IR_MATCH_FENCE1) { _f1 = 1; break; }
            int _sd = 0; for (int i = 0; i < n; i++) if (nodes[i]->op == IR_MATCH_DEFER && nodes[i]->seal == 1) { _sd = 1; break; }
            int _f1r = _f1; { extern int zdp_seam_tier(const IR_t *); const char * _fre = getenv("SCRIP_FENCE_RESUME"); if (!(_fre && *_fre == '0') && g_emit_cfg->body_root && g_emit_cfg->body_root->op != IR_MATCH_FENCE1 && (zdp_seam_tier(g_emit_cfg->body_root) == 1 || zdp_seam_tier(g_emit_cfg->body_root) == 3)) _f1r = 0; }
            if (getenv("SCRIP_RESUME_WHY")) { extern int zdp_seam_tier(const IR_t *); const IR_t * _br = g_emit_cfg->body_root; int _found = 0; for (int i = 0; i < n; i++) if (nodes[i] == _br) _found = 1;
                int _wnc = 0, _wlf = 0, _wfn = 0; sn4_blob_choice_scan(&_wnc, &_wlf, &_wfn);
                fprintf(stderr, "[RESUME-WHY] flat_pat=%d dres=%d f1=%d f1r=%d sd=%d body_root_op=%d tier=%d seal=%d in_nodes=%d n=%d nc=%d lf=%d fn=%d cro=%d\n", g_emit.flat_pat, _dres, _f1, _f1r, _sd, _br ? (int)_br->op : -1, _br ? zdp_seam_tier(_br) : -1, _br ? (int)_br->seal : -1, _found, n, _wnc, _wlf, _wfn, sn4_choice_rbp_off()); }
            if (!_f1r) for (int i = 0; i < n; i++) if (nodes[i] == g_emit_cfg->body_root && resume_carrier_ok(nodes[i], _sd)) { resume_tgt = betas[i]; break; }
            if (!_f1 && !_sd && resume_tgt == &lbl_ω && sn4_alt_carrier() && g_emit_cfg->body_root && (g_emit_cfg->body_root->op == IR_MATCH_ALTERNATE || g_emit_cfg->body_root->op == IR_DISJUNCTION) && !g_emit_cfg->body_root->seal) {
                int _nc = 0, _lf = 0, _fn = 0; sn4_blob_choice_scan(&_nc, &_lf, &_fn);
                if (_nc == 1 && !_fn && (_lf || sn4_choice_rbp_off() != 0)) for (int i = 0; i < n; i++) if (nodes[i] == g_emit_cfg->body_root) { resume_tgt = betas[i]; break; }    } } }
        emit_jmp_label(resume_tgt, JMP_JMP);
    }
    }
    if (g_emit.flat_stmt_frame) { emit_sep_rule('-'); emit_label_define_bb(&lbl_stcγ); bb_emit_x86(bb_glue_framed_leave()); emit_jmp_label(&lbl_γ, JMP_JMP); emit_label_define_bb(&lbl_stcω); bb_emit_x86(bb_glue_framed_leave()); emit_jmp_label(&lbl_ω, JMP_JMP); }
    { extern int g_flat_frame_floor; int _wire_stub = ((g_emit.flat_jmp_entry && g_flat_frame_floor > 0) || (g_emit.flat_lcl_proc && !(g_emit_cfg && g_emit_cfg->icn_cells_graph))) ? 1 : 0;
    int _blob_wire = (!_wire_stub && g_emit.flat_jmp_entry && g_emit.flat_pat) ? 1 : 0;
    if (!bare && !_top_hoist) {
    emit_sep_rule('-'); emit_label_define_bb(&lbl_γ);
    if (g_emit.zframe_graph || (g_emit_cfg && g_emit_cfg->icn_cells_graph && g_emit.flat_lcl_proc && g_emit.flat_jmp_entry)) { extern void xa_flat_zframe_epilogue_γ(void); xa_flat_zframe_epilogue_γ(); }
    else if (_blob_wire) {
        int _bfb = blob_frame_bytes();
        bb_emit_x86(IF(_bfb > 0, x86("comment", "WIRE-STACK (s195) SUSPEND, REGISTER-FREE OF r10/r11: yielding keeps the frame AND the pair, so the resume record is built by reading the banked pair through rcx -- omega pushed first, then gamma, and rcx still carries gamma into the jmp.") + x86("mov", "rcx", RDQ("rbp", -16)) + x86("push", "rbp") + x86("push", "rcx") + x86("mov", "rcx", RDQ("rbp", -8)) + x86("push", "rcx") + x86_lea_ext("rax", &lbl_res) + x86("push", "rax") + x86("mov", "rbp", RDQ("rbp", 0)) + x86_jmp_reg("rcx")) + IF(_bfb <= 0, x86("sub", "rsp", 8L) + x86("push", "r11") + x86("push", "r10") + x86_lea_ext("rax", &lbl_res) + x86("push", "rax") + x86_jmp_reg("r10")));
    }
    else if (g_emit_cfg && g_emit_cfg->icn_cells_graph && g_emit.flat_lcl_proc && !g_emit.flat_jmp_entry) { int _bk = g_emit.flat_frame_bytes;
        if (g_is_text) { char _seg[128]; snprintf(_seg, sizeof _seg, "and rsp, -16\nxor edi, edi\ncall exit@PLT\n"); emit_text_n(_seg, strlen(_seg)); }
        else { ef_b4(0x48, 0x83, 0xE4, 0xF0); ef_b3(0x31, 0xFF, 0x90); { uint64_t _ex = (uint64_t)(uintptr_t)(void *)exit; ef_b2(0x48, 0xB8); bb_emit_u64(_ex); ef_b2(0xFF, 0xD0); } } }
    else { if (xa_flat_class_c_pred()) xa_flat_chain_epilogue(); bb_emit_x86(_wire_stub ? bb_glue_wire_γ() : bb_glue_outer_γ()); }
    }
    if (!bare && !_top_hoist) {
    emit_sep_rule('-'); emit_label_define_bb(&lbl_ω);
    if (g_emit.zframe_graph || (g_emit_cfg && g_emit_cfg->icn_cells_graph && g_emit.flat_lcl_proc && g_emit.flat_jmp_entry)) { extern void xa_flat_zframe_epilogue_ω(void); xa_flat_zframe_epilogue_ω(); }
    else if (g_emit_cfg && g_emit_cfg->icn_cells_graph && g_emit.flat_lcl_proc && !g_emit.flat_jmp_entry) { int _bk = g_emit.flat_frame_bytes;
        if (g_is_text) { char _seg[128]; snprintf(_seg, sizeof _seg, "and rsp, -16\nmov edi, 1\ncall exit@PLT\n"); emit_text_n(_seg, strlen(_seg)); }
        else { ef_b4(0x48, 0x83, 0xE4, 0xF0); ef_b3(0xBF, 0x01, 0x00); ef_b1(0x00); ef_b1(0x00); { uint64_t _ex = (uint64_t)(uintptr_t)(void *)exit; ef_b2(0x48, 0xB8); bb_emit_u64(_ex); ef_b2(0xFF, 0xD0); } } }
    else if (_blob_wire) { extern int sn4_blob_casmark(void); bb_emit_x86(IF(blob_frame_bytes() > 0, IF(sn4_blob_casmark(), x86("mov", "r12", RDQ("rbp", -32))) + x86("mov", "rsp", "rbp") + x86("pop", "rbp")) + x86("comment", "WIRE-STACK (s195) FRETURN FORM, REGISTER-FREE: after mov rsp,rbp;pop rbp the stack is back at entry depth, which is exactly where the caller's PUSHed pair sits -- the RETIRING exit owns the release.  ⛔ The LANDING must NOT release it: a γ-SUSPEND leaves the blob's resume record on top of the pair, so a landing-side add would eat the record instead (Lon s195: yielding is different from returning).") + x86("add", "rsp", 8L) + x86("ret")); }
    else { if (xa_flat_class_c_pred()) xa_flat_chain_epilogue(); bb_emit_x86(_wire_stub ? bb_glue_wire_ω() : bb_glue_outer_ω()); } } }
    { extern int g_flat_dc_np; extern long g_last_dc_off; g_last_dc_off = -1;
      if (g_flat_dc_np >= 0) {
          if (!g_emit.flat_jmp_entry || !g_emit.flat_lex || g_emit.flat_gen || g_emit.flat_pat) { fprintf(stderr, "FATAL PL-DC: driver-armed graph is not det-lexical jmp-entry (jmp=%d lex=%d gen=%d pat=%d)\n", g_emit.flat_jmp_entry, g_emit.flat_lex, g_emit.flat_gen, g_emit.flat_pat); abort(); }
          bb_label_t lbl_dc; emit_label_initf(&lbl_dc, "%s_dcα", fam);
          emit_sep_rule('-'); emit_label_define_bb(&lbl_dc);
          g_emit.flat_dc_body_p = (g_emit.zframe_graph || (g_emit_cfg && g_emit_cfg->icn_cells_graph && g_emit.flat_lcl_proc)) ? &lbl_α : &lbl_α_body;
          { extern void xa_flat_dc_stub(void); xa_flat_dc_stub(); }
          g_emit.flat_dc_body_p = (bb_label_t *)0;
          if (!g_is_text) g_last_dc_off = (long)lbl_dc.offset;
      } }
    if (text_externalise && g_is_text) {
        data_buf_flush_pending_label();
        xa_dispatch(XA_FLAT_DATA_SECTION);
        data_buf_reset();
    }
    return 0;
}
static int g_in_prebuild = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int emit_chain_arity(const IR_t *n) {
    switch (n->op) {
    case IR_LIT_INTEGER: case IR_LIT_STRING: case IR_LIT_REAL:
    case IR_VAR:   case IR_KEYWORD_ICON: case IR_KEYWORD_ICON_GEN: case IR_KEYWORD_SNOBOL4: return 0;
    case IR_BINOP: case IR_TO: return 2;
    case IR_TO_BY: return 3;
    case IR_MAKE_LIST: return n->n_operands;
    case IR_CONJUNCTION: case IR_GOTO: return 0;
    case IR_UNOP: case IR_UNOP_TEST: return 1;
    case IR_ASSIGN: return 1;
    case IR_RETURN: return 1;
    case IR_CALL_PROC_STAGED: case IR_CALL_BUILTIN:
    case IR_CALL_BUILTIN_ICON: case IR_CALL_BUILTIN_SNOBOL4: case IR_CALL_BUILTIN_PROLOG:
    case IR_CALL:  return n->n_operands;
    case IR_PROC_GEN: return 0;
    case IR_SCAN_TAB: case IR_SCAN_MOVE: case IR_SCAN_POS:
    case IR_SCAN_UPTO: case IR_SCAN_ANY: case IR_SCAN_MANY:
    case IR_SCAN_FIND: case IR_SCAN_MATCH: case IR_SCAN_BAL: return 0;
    default:       return -1;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void emit_chain_operand_refs(IR_t *entry) {
    IR_t *chain[8192]; int nc = 0;
    IR_t *seen[8192]; int ns = 0;
    IR_t *stkv[8192]; int sv = 0;
    { int guard = 0; while (entry && (entry->op == IR_SUCCEED || entry->op == IR_FAIL) && entry->γ.node && guard++ < 8192) entry = entry->γ.node; }
    entry = entry;
    stkv[sv++] = entry;
    while (sv > 0 && nc < 8192) {
        IR_t *c = stkv[--sv];
        if (!c || c->op == IR_SUCCEED || c->op == IR_FAIL) continue;
        int dup = 0; for (int i = 0; i < ns; i++) if (seen[i] == c) { dup = 1; break; }
        if (dup) continue;
        seen[ns++] = c; chain[nc++] = c;
        if ((c->op == IR_BINOP) && c->ω.node && sv < 8192) stkv[sv++] = c->ω.node;
        if ((c->op == IR_CALL || ir_is_call_kind(c->op)) && c->ω.node && sv < 8192) stkv[sv++] = c->ω.node;
        if ((c->op == IR_SUBSCRIPT || c->op == IR_RANDOM || c->op == IR_DEREF || c->op == IR_ASSIGN_VAR || c->op == IR_REV_ASSIGN_VAR || c->op == IR_KEYWORD_ASSIGN || c->op == IR_SCAN_TAB || c->op == IR_SCAN_MOVE || c->op == IR_SCAN_POS || c->op == IR_SCAN_MATCH || c->op == IR_SCAN_ANY
             || c->op == IR_SWAP_VAR || c->op == IR_CALL_VALUE || c->op == IR_VAR) && c->ω.node && sv < 8192)
            stkv[sv++] = c->ω.node;
        if (c->γ.node && sv < 8192) stkv[sv++] = c->γ.node;
    }
    for (int i = 0; i < nc; i++) if (ir_is_generator_kind(chain[i]->op) && chain[i]->ω.node) {
        int present = 0; for (int j = 0; j < ns; j++) if (seen[j] == chain[i]->ω.node) { present = 1; break; }
        if (!present && sv < 8192) stkv[sv++] = chain[i]->ω.node; }
    while (sv > 0 && nc < 8192) {
        IR_t *c = stkv[--sv];
        if (!c || c->op == IR_SUCCEED || c->op == IR_FAIL) continue;
        int dup = 0; for (int i = 0; i < ns; i++) if (seen[i] == c) { dup = 1; break; }
        if (dup) continue;
        seen[ns++] = c; chain[nc++] = c;
        if ((c->op == IR_BINOP) && c->ω.node && sv < 8192) stkv[sv++] = c->ω.node;
        if ((c->op == IR_CALL || ir_is_call_kind(c->op)) && c->ω.node && sv < 8192) stkv[sv++] = c->ω.node;
        if ((c->op == IR_SUBSCRIPT || c->op == IR_RANDOM || c->op == IR_DEREF || c->op == IR_ASSIGN_VAR || c->op == IR_REV_ASSIGN_VAR || c->op == IR_KEYWORD_ASSIGN || c->op == IR_SCAN_TAB || c->op == IR_SCAN_MOVE || c->op == IR_SCAN_POS || c->op == IR_SCAN_MATCH || c->op == IR_SCAN_ANY
             || c->op == IR_SWAP_VAR || c->op == IR_CALL_VALUE || c->op == IR_VAR) && c->ω.node && sv < 8192)
            stkv[sv++] = c->ω.node;
        if (ir_is_generator_kind(c->op) && c->ω.node && sv < 8192) stkv[sv++] = c->ω.node;
        if (c->γ.node && sv < 8192) stkv[sv++] = c->γ.node;
    }
    IR_t *stk[512]; int sp = 0;
    for (int i = 0; i < nc; i++) {
        IR_t *n = chain[i];
        switch (n->op) { case IR_RETURN: case IR_SUSPEND: case IR_CORET: case IR_COFAIL: sp = 0; continue; default: break; }
        int ar = emit_chain_arity(n);
        if (ar < 0) { sp = 0; continue; }
        else if (ar >= 1 && sp >= ar) { if (n->n_operands < ar) { n->n_operands = 0; for (int k = ar; k >= 1; k--) ir_operand_push(n, stk[sp - k]); } sp -= ar; }
        else if (ar >= 1) { sp = 0; }
        stk[sp++] = n;
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
    int capen; { static int v = -1; if (v < 0) { const char *e = getenv("SCRIP_CAP_NOFILL"); v = e ? (atoi(e) != 0) : 1; } capen = v; }
    g_emit.flat_cap_n = 0; int capovf = !capen;
    if (g) for (int i = 0; i < g->n; i++) { IR_t *nd = g->all[i]; if (nd && nd->op == IR_MATCH_ASSIGN_SAVE) { int off = nd_slot(nd); if (off < 0) continue;
        if (!capovf) { int dup = 0; for (int j = 0; j < g_emit.flat_cap_n; j++) if (g_emit.flat_cap_off[j] == off) { dup = 1; break; } if (dup) continue;
            if (g_emit.flat_cap_n < 48) { g_emit.flat_cap_off[g_emit.flat_cap_n++] = off; continue; }
            capovf = 1; for (int j = 0; j < g_emit.flat_cap_n; j++) if (so < 0 || g_emit.flat_cap_off[j] < so) so = g_emit.flat_cap_off[j]; g_emit.flat_cap_n = 0; }
        if (so < 0 || off < so) so = off; } }
    g_emit.flat_layout_unknown = 0;
    if (rg <= 0) { rg = 4096; so = -1; g_emit.flat_layout_unknown = 1; }
    { extern int g_flat_frame_floor; if (g_flat_frame_floor > 0 && rg < g_flat_frame_floor) { rg = g_flat_frame_floor; so = -1; g_emit.flat_layout_unknown = 1; } }
    g_emit.flat_jmp_entry = 1; g_emit.flat_frame_bytes = (48 + (g_emit_cfg ? g_emit_cfg->jcon_value_region : 0) + 15) & ~15;
    g_emit.flat_seed_off = (so >= 16 && so <= rg) ? so : 0;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" int emit_jmp_entry_for_patproc(const char *pname, IR_graph_t *g) {
    if (!pname || strncmp(pname, "PAT$", 4) != 0) return 0;
    g_emit.flat_pat = 1;
    return emit_jmp_entry_arm_region(g);
}
int  g_flat_frame_floor = 0;
int  g_flat_dc_np = -1;
long g_last_dc_off = -1;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" int zop_audit_seen(void) { return g_emit.zop_seen; }
extern "C" void zop_audit_seen_clear(void) { g_emit.zop_seen = 0; }
extern "C" void emit_jmp_entry_clear(void) { extern int g_flat_frame_floor; g_flat_frame_floor = 0; g_emit.flat_jmp_entry = 0; g_emit.flat_frame_bytes = 0; g_emit.flat_seed_off = 0; g_emit.flat_layout_unknown = 0; g_emit.flat_pat = 0; g_emit.flat_lex = 0; g_emit.flat_gen = 0; g_emit.flat_deep_arrival = 0; g_emit.flat_cap_n = 0; g_flat_dc_np = -1; g_emit.flat_lcl_proc = 0; g_emit.zframe_graph = 0; }
static int emit_graph_has_suspend(IR_graph_t *g) { if (!g) return 0; for (int i = 0; i < g->n; i++) if (g->all[i] && g->all[i]->op == IR_SUSPEND) return 1; return 0; }
static int emit_graph_has_deep_arrival(IR_graph_t *g) {    if (!g) return 1; for (int i = 0; i < g->n; i++) { IR_t *c = g->all[i]; if (!c) continue; switch (c->op) { case IR_SUSPEND: case IR_SCAN: case IR_SCAN_ENTER: case IR_SCAN_ALTERNATE: case IR_SCAN_SEQUENCE: case IR_SCAN_UPTO: case IR_SCAN_FIND: case IR_SCAN_BAL: case IR_SCAN_MATCH: case IR_SCAN_MOVE: case IR_SCAN_TAB: case IR_TO: case IR_TO_BY: case IR_LIMIT: case IR_REPALT: case IR_PROC_GEN: case IR_CREATE: case IR_ITERATE: case IR_DISJUNCTION: case IR_CALL_BUILTIN_GEN: case IR_KEYWORD_ICON_GEN: case IR_MATCH_FENCE1: case IR_MATCH_ABORT: case IR_MATCH_ARBNO: case IR_MATCH_CALLOUT: case IR_MATCH_VALUE: return 1; case IR_MATCH_DEFER: return 1;    default: break; } } return 0; }
static IR_t * g_fbm_nd[8192]; static unsigned char g_fbm_bit[8192]; static int g_fbm_n = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int emit_fb_bit_of(IR_t * nd) { for (int i = 0; i < g_fbm_n; i++) if (g_fbm_nd[i] == nd) return g_fbm_bit[i]; return 1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int emit_fb_stmt_scan(IR_graph_t * g) {
    static int fbdbg = -1; if (fbdbg < 0) { const char * e = getenv("SCRIP_FB_DEBUG"); fbdbg = (e && *e == '1') ? 1 : 0; }
    g_fbm_n = 0; if (!g || g->n <= 0 || g->n > 8192) { if (fbdbg) fprintf(stderr, "[FB-STMT] bail size g=%p n=%d\n", (void *)g, g ? g->n : -1); return 0; }
    for (int i = 0; i < g->n; i++) { IR_t * c = g->all[i]; if (!c) continue; switch (c->op) {
        case IR_SUSPEND: case IR_SCAN: case IR_SCAN_ENTER: case IR_SCAN_ALTERNATE: case IR_SCAN_SEQUENCE: case IR_SCAN_UPTO: case IR_SCAN_FIND: case IR_SCAN_BAL: case IR_SCAN_MATCH: case IR_SCAN_MOVE: case IR_SCAN_TAB:
        case IR_TO: case IR_TO_BY: case IR_LIMIT: case IR_REPALT: case IR_PROC_GEN: case IR_CREATE: case IR_ITERATE: case IR_DISJUNCTION: case IR_CALL_BUILTIN_GEN: case IR_KEYWORD_ICON_GEN:
        case IR_MATCH_ABORT: case IR_MATCH_CALLOUT: { if (fbdbg) { extern const char * bb_op_name(IR_e); fprintf(stderr, "[FB-STMT] bail kind %s @%d n=%d\n", bb_op_name(c->op), i, g->n); } return 0; }
        default: break; } }
    for (int i = 0; i < g->n; i++) { IR_t * c = g->all[i]; g_fbm_nd[g_fbm_n] = c; g_fbm_bit[g_fbm_n] = (c && ir_is_matcher(c->op)) ? 1 : 0; g_fbm_n++; }
    for (int h = 0; h < g->n; h++) { IR_t * hn = g->all[h]; if (!hn || hn->op != IR_MATCH_BEGIN) continue; int e = -1, deep = 0;
        for (int k = h + 1; k < g->n; k++) { IR_t * c = g->all[k]; if (!c) continue;
            if (c->op == IR_MATCH_DEFER || c->op == IR_MATCH_ARBNO || (c->op == IR_MATCH_FENCE1 || c->op == IR_MATCH_FENCE0) || c->op == IR_MATCH_VALUE) deep = 1;
            if (c->op == IR_MATCH_END || c->op == IR_MATCH_REPLACE) { e = k; break; } }
        if (e < 0) { if (fbdbg) fprintf(stderr, "[FB-STMT] bail unterminated HEAD @%d n=%d\n", h, g->n); return 0; }
        if (deep) { for (int k = h; k <= e; k++) g_fbm_bit[k] = 1; } }
    if (fbdbg) { int z = 0; for (int i = 0; i < g_fbm_n; i++) if (!g_fbm_bit[i]) z++; fprintf(stderr, "[FB-STMT] refine=1 n=%d rsp_nodes=%d\n", g_fbm_n, z); }
    return 1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int emit_stmt_frame_scan(IR_graph_t * g, IR_t * entry) {
    if (!g || !entry || g->n <= 0 || g->n > 8192) return 0;
    static int _sd = -1; if (_sd < 0) { const char * e = getenv("SCRIP_STF_DEBUG"); _sd = (e && *e == '1') ? 1 : 0; }
    enum { STF_MAX = 8192 };
    IR_t ** q = (IR_t **)alloca(sizeof(IR_t *) * STF_MAX); IR_t ** seen = (IR_t **)alloca(sizeof(IR_t *) * STF_MAX); int qh = 0, qt = 0, sn = 0;
    q[qt++] = entry;
    while (qh < qt) { IR_t * c = q[qh++];
        if (!c || c->op == IR_SUCCEED || c->op == IR_FAIL) continue;
        { int dup = 0; for (int i = 0; i < sn; i++) if (seen[i] == c) { dup = 1; break; } if (dup) continue; }
        if (sn >= STF_MAX || qt >= STF_MAX - 2) { if (_sd) fprintf(stderr, "[STF] reject overflow sn=%d qt=%d\n", sn, qt); return 0; }
        seen[sn++] = c;
        if ((c->γ.node && c->γ.node == entry) || (c->ω.node && c->ω.node == entry)) { if (_sd) fprintf(stderr, "[STF] reject BACKWARD EDGE TO FIRST HEAD sn=%d\n", sn); return 0; }
        switch (c->op) { case IR_LIT_INTEGER: case IR_LIT_STRING: case IR_ASSIGN: break;
            case IR_VAR: case IR_BINOP: if (c->ω.node) q[qt++] = c->ω.node; break;
            default: if (c->ω.node) q[qt++] = c->ω.node; break; }
        { extern int fc_geom(const IR_t *, long *); long _fk = 0; int _fc = fc_geom(c, &_fk); if (_sd) { extern const char * bb_op_name(IR_e); fprintf(stderr, "[STF] node %s fc=%d k=%ld\n", bb_op_name(c->op), _fc, _fk); }
          static int _c2c = -1; if (_c2c < 0) { const char * e2 = getenv("SCRIP_CALL2BB"); _c2c = (e2 && *e2 == '1') ? 1 : 0; }
          int _callfam = (_c2c && (c->op == IR_CALL || c->op == IR_CALL_PROC_STAGED || (c->op == IR_DEFINE && ir_define_sr_citizen(c)) || c->op == IR_GOTO_DEFERRED)) ? 1 : 0;
          { extern int fc_vread_fp(const IR_t *); if (!_fc && c->op == IR_MATCH_BEGIN && fc_vread_fp(c) >= 0) _fc = 1;
            if (!_fc && c->op == IR_MATCH_END && c->n_operands > 0 && c->operands[0] && fc_vread_fp(c->operands[0]) >= 0) _fc = 1;
            if (!_fc && c->op == IR_MATCH_LIT) _fc = 1; }
          int _stfwire = (c->op == IR_GOTO || c->op == IR_STATEMENT_BEGIN || c->op == IR_STATEMENT_END) ? 1 : 0;
          if (!_fc && !(c->op == IR_BINOP || c->op == IR_ASSIGN || _stfwire || _callfam)) { if (_sd) { extern const char * bb_op_name(IR_e); fprintf(stderr, "[STF] reject UNREGISTERED %s sn=%d\n", bb_op_name(c->op), sn); } return 0; } }
        if (c->γ.node) q[qt++] = c->γ.node; }
    if (_sd) fprintf(stderr, "[STF] ADMIT reachable=%d (g->n=%d)\n", sn, g->n);
    return sn > 0 ? 1 : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int emit_graph_zstatic(IR_graph_t *g) { if (!g) return 0; for (int i = 0; i < g->n; i++) { IR_t *c = g->all[i]; if (c && (c->op == IR_MATCH_DEFER || c->op == IR_MATCH_VALUE)) return 0; } return 1; }
static int emit_graph_uniform(IR_graph_t *g) { if (!emit_graph_zstatic(g)) return 0; for (int i = 0; i < g->n; i++) { IR_t *c = g->all[i]; if (c && c->op == IR_MATCH_ARBNO) return 0; } return 1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" int emit_jmp_entry_for_proc(const char *pname, int dyn_scope, int is_generator, IR_graph_t *g) {
    if (pname && strncmp(pname, "gram__", 6) == 0) return 0;
    int is_lbl = pname && strncmp(pname, "LBL__", 5) == 0;
    g_emit.flat_lex = (!dyn_scope && !is_lbl) ? 1 : 0;
    g_emit.flat_gen = (is_generator && emit_graph_has_suspend(g)) ? 1 : 0;
    return emit_jmp_entry_arm_region(g);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" int emit_jmp_entry_for_chain(IR_graph_t *g) {
    extern int g_flat_dc_np; g_flat_dc_np = -1;
    return emit_jmp_entry_arm_region(g);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
bb_box_fn emit_chain(IR_t *entry, FILE *out, const char *prefix) {
    if (!entry) return NULL;
    emit_chain_mark_entry_emitted(entry);
    emit_chain_operand_refs(entry);
    if (g_emit_cfg) zls_fct_finalize(g_emit_cfg, 1);
    g_bb_slotmap_n = 0;
    g_flat_chain_set_n = 0;
    { extern void bb_scc_handoff_reset(void); bb_scc_handoff_reset(); }
    { extern int g_scan_regs_live; g_scan_regs_live = 0; }
    { extern void zop_audit_graph_close(void); zop_audit_graph_close(); g_emit.zop_seen = 0; }
    g_emit.flat_all_zd = 0;
    g_emit.flat_deep_arrival = emit_graph_has_deep_arrival(g_emit_cfg);
    { int _rc_own = (g_emit_cfg && g_emit_cfg->resumable_callable) ? 1 : 0; g_emit.flat_fb_refine = (g_emit.flat_deep_arrival && !g_emit.flat_pat && !g_emit.flat_gen && !_rc_own) ? emit_fb_stmt_scan(g_emit_cfg) : 0;
      { static int fbdbg2 = -1; if (fbdbg2 < 0) { const char * e = getenv("SCRIP_FB_DEBUG"); fbdbg2 = (e && *e == '1') ? 1 : 0; } if (fbdbg2 && !g_emit.flat_fb_refine) fprintf(stderr, "[FB-STMT] hook skip deep=%d pat=%d gen=%d gp=%d rc=%d n=%d\n", g_emit.flat_deep_arrival, g_emit.flat_pat, g_emit.flat_gen, g_gen_proc_active, g_resumable_callable_active, g_emit_cfg ? g_emit_cfg->n : -1); } }
    { static int _stf = -1; if (_stf < 0) { const char * e = getenv("SCRIP_STMT_FRAME"); _stf = (e && *e == '1') ? 1 : 0; }
      extern int g_flat_outer_nparams; extern int g_flat_frame_floor; g_emit.flat_outer_nparams = g_flat_outer_nparams;
      { static int _gfr = -1; if (_gfr < 0) { const char * _e = getenv("SCRIP_ICN_GENFRAME"); _gfr = (_e && *_e == '1') ? 1 : 0; }
        int _gen_ok = (!g_emit.flat_gen) || (_gfr && g_emit_cfg && g_emit_cfg->icn_cells_graph);
        g_emit.flat_lcl_proc = (!g_emit.flat_pat && _gen_ok && g_emit_cfg && ((g_emit.flat_jmp_entry && (g_emit_cfg->nparams > 0 || g_emit_cfg->nlocals > 0)) || g_emit_cfg->icn_cells_graph)) ? 1 : 0; }
      g_emit.zframe_graph = (g_emit_cfg && g_emit_cfg->zframe_graph && !g_emit_cfg->icn_cells_graph) ? 1 : 0;
      if ((g_emit.zframe_graph || (g_emit_cfg && g_emit_cfg->icn_cells_graph)) && g_emit.flat_frame_bytes == 0) { g_emit.flat_frame_bytes = (48 + (g_emit_cfg ? g_emit_cfg->jcon_value_region : 0) + 15) & ~15; }
      int _c2g; { static int _c2 = -1; if (_c2 < 0) { const char * e = getenv("SCRIP_CALL2BB"); _c2 = (e && *e == '1') ? 1 : 0; } _c2g = _c2; }
      int _stfj = (_stf && _c2g && g_emit.flat_jmp_entry && !g_emit.flat_pat && !g_emit.flat_gen && !g_emit.flat_deep_arrival && !g_gen_proc_active && !(g_emit_cfg && g_emit_cfg->resumable_callable) && g_flat_frame_floor > 0) ? 1 : 0;
      int _pat_ok = (!g_emit.flat_pat) ? 1 : 0;
      g_emit.flat_stmt_frame = ((_stf && !g_emit.flat_jmp_entry && _pat_ok && !g_emit.flat_gen && !g_gen_proc_active && !(g_emit_cfg && g_emit_cfg->resumable_callable) && g_emit.flat_outer_nparams == 0) || _stfj) ? emit_stmt_frame_scan(g_emit_cfg, entry) : 0;
      { static struct { const void * g; const void * e; int v; } _sv[1024]; static int _sn = 0; static int _strict = -1; if (_strict < 0) { const char * e = getenv("SCRIP_STF_STRICT"); _strict = (e && *e == '1') ? 1 : 0; } if (_stf && g_emit_cfg) { int _f = -1; for (int i = 0; i < _sn; i++) if (_sv[i].g == (const void *)g_emit_cfg) { _f = i; break; }
        if (_f < 0) { if (_sn < 1024) { _sv[_sn].g = (const void *)g_emit_cfg; _sv[_sn].e = (const void *)entry; _sv[_sn].v = g_emit.flat_stmt_frame; _sn++; } }
        else { if (_sv[_f].v != g_emit.flat_stmt_frame) { fprintf(stderr, "WARN STF-STABILITY: graph %p verdict %d then %d -- the per-graph verdict is NOT stable across emissions; ZLEAK-3's plan-time fixpoint would mis-decide here (witness: 1014_func_freturn under CALL2BB)\n", (const void *)g_emit_cfg, _sv[_f].v, g_emit.flat_stmt_frame); if (_strict) abort(); }
              if (g_emit.flat_stmt_frame && _sv[_f].e != (const void *)entry) { fprintf(stderr, "WARN STF-SINGLE-ENTRY: ARMED graph %p emitted from entries %p and %p -- arming-implies-single-entry violated\n", (const void *)g_emit_cfg, _sv[_f].e, (const void *)entry); if (_strict) abort(); } } } }
      if (g_emit.flat_stmt_frame && _stfj) { g_emit.flat_frame_bytes = 48; g_emit.flat_seed_off = 0; g_emit.flat_layout_unknown = 0; g_emit.flat_cap_n = 0; }
      { static int _sd2 = -1; if (_sd2 < 0) { const char * e = getenv("SCRIP_STF_DEBUG"); _sd2 = (e && *e == '1') ? 1 : 0; } if (_sd2 && _stf) fprintf(stderr, "[STF] choke stf=%d jmp=%d pat=%d gen=%d gp=%d rc=%d np=%d deep=%d floor=%d c2=%d stfj=%d -> live=%d g=%p entry=%p\n", _stf, g_emit.flat_jmp_entry, g_emit.flat_pat, g_emit.flat_gen, g_gen_proc_active, (g_emit_cfg && g_emit_cfg->resumable_callable) ? 1 : 0, g_emit.flat_outer_nparams, g_emit.flat_deep_arrival, g_flat_frame_floor, _c2g, _stfj, g_emit.flat_stmt_frame, (void*)g_emit_cfg, (void*)entry); } }
    g_last_flat_frame_bytes = g_emit_cfg ? g_emit_cfg->jcon_value_region : 0;
    g_last_flat_zstatic = (g_emit_cfg && !g_emit.flat_layout_unknown) ? emit_graph_zstatic(g_emit_cfg) : 0;
    g_last_flat_fp = (g_emit_cfg && !g_emit.flat_layout_unknown) ? zls_g_fp_total(g_emit_cfg) : 0;
    g_last_flat_uniform = (g_emit_cfg && !g_emit.flat_layout_unknown) ? emit_graph_uniform(g_emit_cfg) : 0;
    if (out) { emitter_init_text(out, TEXT_MODE_INVOCATION); int rc = codegen_flat_chain_body(entry, prefix); emitter_end(); return rc == 0 ? (bb_box_fn)1 : NULL; }
    bb_buf_t buf = bb_alloc(FLAT_BUF_MAX);
    if (!buf) return NULL;
    g_flat_node_id = 0;
    emitter_init_binary(buf, FLAT_BUF_MAX);
    codegen_flat_chain_body(entry, prefix);
    if (g_emit_chain_posthook) { void (*h)(void) = g_emit_chain_posthook; g_emit_chain_posthook = NULL; h(); }
    int nbytes = emitter_end();
    extern int bb_emit_overflow;
    if (bb_emit_overflow || nbytes <= 0 || nbytes > FLAT_BUF_MAX) {
        extern int bb_emit_pos; extern int bb_patch_count;
        fprintf(stderr, "[IBB] emit_chain('%s') FAILED: overflow=%d nbytes=%d pos=%d cap=%d patches=%d/%d — %s\n",
                prefix ? prefix : "", bb_emit_overflow, nbytes, bb_emit_pos, FLAT_BUF_MAX, bb_patch_count, BB_PATCH_MAX,
                (bb_emit_pos >= FLAT_BUF_MAX) ? "FLAT_BUF_MAX exceeded (graph too large for the 1MB flat buffer)" :
                (bb_patch_count >= BB_PATCH_MAX) ? "BB_PATCH_MAX exceeded" : "empty or invalid emission");
        bb_free(buf, FLAT_BUF_MAX); return NULL;
    }
    bb_seal(buf, (size_t)nbytes);
    bb_pool_trim_last(buf, FLAT_BUF_MAX, (size_t)nbytes);
    return (bb_box_fn)buf;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void gva_collect_graph(IR_graph_t *g) { if (!g) return; for (int i = 0; i < g->n; i++) { IR_t *nd = g->all[i]; if (!nd) continue; if ((nd->op == IR_VAR || nd->op == IR_ASSIGN || nd->op == IR_VAR_REF) && IR_LIT(nd).sval) { const char *_gn = IR_LIT(nd).sval; if (!strncmp(_gn, "PATV$", 5)) continue;    (void)gva_collect_var(_gn); } } }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void gva_collect_icon_globals(void) {
    extern const char *global_names[]; extern int global_count;
    for (int i = 0; i < global_count; i++) if (global_names[i] && strncmp(global_names[i], "PATV$", 5) != 0) (void)gva_collect_var(global_names[i]);
}
#define PL_CATCH_MAX 64
static IR_t *g_pl_catch_nodes[PL_CATCH_MAX];
static int   g_pl_catch_n = 0;
static std::string g_text_acc;
static FILE * g_emit_sink = NULL;
static long   g_emit_text_count = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void emit_set_sink(FILE * out) { g_emit_sink = out; }
long emit_text_count(void) { return g_emit_text_count; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void emit_text_raw_n(const char * s, size_t n) {
    if (!s || n == 0) return;
    g_emit_text_count += (long)n;
    if (g_emit_sink) fwrite(s, 1, n, g_emit_sink);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void emit_text_n(const char * s, size_t n) {
    if (!s || n == 0) return;
    if (MEDIUM_BINARY) { emit_text_raw_n(s, n); return; }
    g_text_acc.append(s, n);
    size_t last = g_text_acc.rfind('\n');
    if (last == std::string::npos) return;
    std::string done = x86_4col(g_text_acc.substr(0, last + 1));
    g_text_acc.erase(0, last + 1);
    emit_text_raw_n(done.data(), done.size());
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void emit_text_flush(void) {
    if (g_text_acc.empty()) return;
    std::string done = x86_4col(g_text_acc); g_text_acc.clear();
    emit_text_raw_n(done.data(), done.size());
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void emit_textf(const char * fmt, ...) {
    va_list ap; va_start(ap, fmt); char sb[4096]; int need = vsnprintf(sb, sizeof sb, fmt, ap); va_end(ap);
    if (need <= 0) return;
    if (need < (int) sizeof sb) emit_text_n(sb, (size_t) need);
    else { char * hb = (char *) malloc((size_t) need + 1); if (!hb) return; va_start(ap, fmt); vsnprintf(hb, (size_t) need + 1, fmt, ap); va_end(ap); emit_text_n(hb, (size_t) need); free(hb); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void emit_textf_flush(void) { emit_text_flush(); }
#ifdef __cplusplus
}
#endif
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" int zzone_off_cur_for(int customer) { return g_emit.node ? zzone_off_for(g_emit.node, customer) : -1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void zzone_disagree(int customer, int staged, int planned) {
    static int bm = -1; if (bm < 0) { const char * e = getenv("SCRIP_ZONE_BOMB"); bm = (e && *e && *e != '0') ? (*e == '2' ? 2 : 1) : 0; }
    if (!bm) return; if (bm == 1 && staged == planned) return;
    static const char * cn[ZCUS_N] = { "LEAF", "ARBNO", "CAPTURE", "FENCE", "CHOICE" };
    fprintf(stderr, "[ZONEBOMB] op=%s customer=%s staged=%d planned=%d\n", g_emit.node ? bb_op_name(g_emit.node->op) : "?", (customer >= 0 && customer < ZCUS_N) ? cn[customer] : "?", staged, planned);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" int zzone_tier_of_cur(void) { extern int zzone_tier_of(const IR_t *); return g_emit.node ? zzone_tier_of(g_emit.node) : 2; }
extern "C" int zzone_off_of_cur(void) { extern int zzone_off_of(const IR_t *); return g_emit.node ? zzone_off_of(g_emit.node) : -1; }

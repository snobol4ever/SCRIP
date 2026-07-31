#ifdef __cplusplus
#include "emit.h"
#include "zeta_choices.h"
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void emit_label_pool_reset(void)
{
    for (int i = 0; i < g_label_pool_n; i++) free(g_label_pool[i]);
    g_label_pool_n = 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * flat_label_kind(IR_e op) { const char * n = bb_op_name(op); static char b[48]; int j = 0; if (n && n[0] == 'I' && n[1] == 'R' && n[2] == '_') n += 3; if (!n) { snprintf(b, sizeof b, "op%d", (int)op); return b; } for (; n[j] && j < 47; j++) b[j] = (n[j] >= 'A' && n[j] <= 'Z') ? (char)(n[j] - 'A' + 'a') : n[j]; b[j] = 0; return b; }
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static bb_label_t *g_lblfold_from[4096]; static bb_label_t *g_lblfold_to[4096]; static int g_lblfold_n = 0;
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
    case XA_FLAT_PROLOGUE:         xa_flat_prologue();         return;
    case XA_FLAT_EPILOGUE:         xa_flat_epilogue();         return;
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
void strtab_reset(void) { g_strtab_n = 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int strtab_intern(const char *s)
{
    if (!s) s = "";
    for (int i = 0; i < g_strtab_n; i++)
        if (g_strtab[i].s == s || strcmp(g_strtab[i].s, s) == 0) return g_strtab[i].idx;
    if (g_strtab_n >= SMX_STRTAB_CAP) { fprintf(stderr, "strtab overflow\n"); abort(); }
    int idx = g_strtab_n;
    g_strtab[g_strtab_n].s = s; g_strtab[g_strtab_n].idx = idx; g_strtab_n++;
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define SMX_CSETTAB_CAP 256
static struct { const char *s; int idx; unsigned char tbl[256]; } g_csettab[SMX_CSETTAB_CAP];
static int g_csettab_n = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void csettab_reset(void) { g_csettab_n = 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const void *csettab_label(char *buf, size_t bufsz, const char *cset)
{
    if (!cset) cset = "";
    for (int i = 0; i < g_csettab_n; i++)
        if (g_csettab[i].s == cset || strcmp(g_csettab[i].s, cset) == 0) { snprintf(buf, bufsz, ".C%d", g_csettab[i].idx); return g_csettab[i].tbl; }
    if (g_csettab_n >= SMX_CSETTAB_CAP) { fprintf(stderr, "csettab overflow\n"); abort(); }
    int idx = g_csettab_n;
    g_csettab[idx].s = cset; g_csettab[idx].idx = idx;
    memset(g_csettab[idx].tbl, 0, 256);
    for (const unsigned char *p = (const unsigned char *)cset; *p; ++p) g_csettab[idx].tbl[*p] = 1;
    g_csettab_n++;
    snprintf(buf, bufsz, ".C%d", idx);
    return g_csettab[idx].tbl;
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
        for (int j = 0; j < 256; j += 16) {
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
#define FLAT_BUF_MAX  (1024 * 1024)
int g_flat_node_id   = 0;
static int g_seq_static_cur = 0;
int g_last_flat_frame_bytes = 0;
int g_last_flat_zstatic = 0;   /* PS-1b (s151): 1 iff the just-emitted graph is DEFER/VALUE-free AND its zls region was known (not the 4096 fallback) -- the emit-side twin of bb_graph_zstatic, captured per-proc by the driver into proc_zstatic_buf and registered so SNO$MKPAT-minted DT_P carry a trustworthy zstatic; default 0 = conservative (chain path). */
int g_last_flat_fp = 0;        /* PS-3 (s152): the just-emitted graph's total FORTH port-cell footprint (fct_fp_range over the whole graph, ALT = 16+fpmax per S10d) -- the interior-cell term of the DT_P suspension ζ size; fed with frame_bytes into emit_patzeta_register by the driver proc loops, both modes. */
int g_last_flat_uniform = 0;   /* PS-3 (s152): 1 iff the just-emitted graph's suspension footprint is the SAME every activation -- zstatic (no DEFER/VALUE) AND no interior ARBNO (retained elements = variable-count carve, the s152 strengthening over PS-1b's predicate) AND region known.  The license for the ARBNO defer-tail constant. */
extern "C" int zls_g_fp_total(IR_graph_t *);
extern "C" int zls_node_has_fields(const IR_t *);
extern "C" int emit_patzeta_lookup(const char *, int *);
extern "C" void zls_fct_finalize(IR_graph_t *, int);   /* PS-3 s153: late fct finalize (defer-bearing tail candidates resolve at emit_chain entry, registry fed) */
extern "C" int fc_tail_defer_susp_g(IR_graph_t *, const IR_t *);   /* PS-3 s153: priced tail-leaf defer query -> arms the zero-guarded β */
extern "C" int fc_tail_dfr(const IR_t *);   /* PS-3 s153: candidate carries priced defer leaves -> tail α zeroes the phantom pad */
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int fbdiv_n = 0; static int fbdiv_mode = -1; static int fbdiv_hooked = 0;
static void fbdiv_report(void) { fprintf(stderr, "FB-DIVERGE TOTAL=%d\n", fbdiv_n); }
static int emit_fb_bit_of(IR_t * nd);
static int emit_fb_stmt_scan(IR_graph_t * g);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void emit_fb_divergence_check(void) { if (fbdiv_mode < 0) { const char * e = getenv("SCRIP_FB_DIVERGE"); fbdiv_mode = (e && *e == '1') ? 1 : 0; } if (!fbdiv_mode) return; if (!fbdiv_hooked) { fbdiv_hooked = 1; atexit(fbdiv_report); } if (emit_jmp_pin_rbp() == emit_rec_pin()) return; fbdiv_n++; fprintf(stderr, "FB-DIVERGE #%d graph=%s data_fb=%s rec_fb=%s deep=%d pat=%d gen=%d genproc=%d rescall=%d\n", fbdiv_n, g_emit.flat_lbl_α ? g_emit.flat_lbl_α : "<anon>", emit_jmp_pin_rbp() ? "rbp" : "rsp", emit_rec_pin() ? "rbp" : "rsp", g_emit.flat_deep_arrival, g_emit.flat_pat, g_emit.flat_gen, g_gen_proc_active, g_resumable_callable_active); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
void bb_prepare(IR_t *nd) {
    if (!PLATFORM_X86) return;
    g_emit.bb_ls = NULL;
    g_emit.bb_rs = NULL;
    g_emit.bb_op_lbl = NULL;
    g_emit.bb_lk = -1;
    (void) nd;
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
    if (op == BINOP_CONCAT)               return BINOP_CAT_CONCAT;
    if (op == BINOP_XREP)                 return BINOP_CAT_XREP;
    if (op == BINOP_ADD || op == BINOP_SUB || op == BINOP_MUL || op == BINOP_DIV || op == BINOP_MOD || op == BINOP_POW) return BINOP_CAT_ARITH;
    if (op == BINOP_CUNION || op == BINOP_CDIFF || op == BINOP_CINTER) return BINOP_CAT_ARITH;
    return BINOP_CAT_ARITH;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" int zls_off(const IR_t *);
static int nd_slot(const IR_t *nd) { return nd ? zls_off(nd) : -1; }
int emit_binop_opnd_slot(IR_t *o) {
    if (!o) return -1;
    extern int is_global(const char *);
    if (o->op == IR_VAR && IR_LIT(o).sval && IR_LIT(o).sval[0] != '&' && (!is_global(IR_LIT(o).sval) || graph_has_local(g_emit_cfg, IR_LIT(o).sval))) { int voff = bb_varslot_peek(IR_LIT(o).sval); if (voff >= 0) return voff; }
    int s = bb_slot_get(o); if (s >= 0) return s;
    return nd_slot(o);
}
static int binop_operand_real_static(IR_graph_t *g, IR_t *o, int depth);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t *g_bnr_vnode[4096]; static int g_bnr_vn = 0; static const char *g_bnr_vname[4096]; static int g_bnr_vnn = 0;
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
    if (op == BINOP_CUNION || op == BINOP_CDIFF || op == BINOP_CINTER) return 1;
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
    if (bb_slot_get(a0) >= 0) return 1;
    int wintexpr = (a0->op == IR_BINOP || a0->op == IR_LIT_INTEGER || a0->op == IR_TO || a0->op == IR_TO_BY || a0->op == IR_VAR || a0->op == IR_CALL || ir_is_call_kind(a0->op));
    if (wintexpr && (a0->op == IR_BINOP || a0->op == IR_TO || a0->op == IR_TO_BY)) return (a0->op == IR_BINOP && IR_LIT(a0).ival == BINOP_CONCAT) ? 2 : 3;
    if (wintexpr) return 4;
    if (a0->op == IR_LIT_STRING && IR_LIT(a0).sval) return 5;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" int zls2_geom(const IR_t *, int, int *, long *);   /* zeta_storage.c — ZLS2 port-hook frame-protocol geometry (slot/K/ops per node) */
extern "C" int fc_geom(const IR_t *, long *);                 /* zeta_storage.c — ZC_PORT_FORTH per-box fixed-cell geometry (RUNG ZB-FC-0) */
extern "C" int fc_cond_fp(const IR_t *);                      /* zeta_storage.c — ZB-FC-3c COND/IMM cross-box displacement = fp(inner); -1 = ungranted */
extern "C" int fc_head_fp(const IR_t *);                      /* zeta_storage.c — ZB-FC-3d statement grant: fp(pattern) for HEAD's window + RELEASE's cross-box reads; -1 = ungranted */
extern "C" int fc_leaf_disp(const IR_t *);                    /* zeta_storage.c — R12-ERAD s65: per-leaf flat displacement under ZC_FRAME_RSP; -1 = unregistered (deliver 0) */
extern "C" int fc_seq_active(const IR_t *);                   /* zeta_storage.c — ZB-FC-3b: this SEQUENCE is FORTH-converted (zero LOCALS; sigma/phi become static edge re-points) */
extern "C" int fc_call_active(const IR_t *);                  /* zeta_storage.c — CALL2BB 3b: registered value-spine call (RESULT window at the call dispatch, ARG window at the paired sr0 dispatch) */
extern "C" int zls_arbno_geom(const IR_t *, int *, int *);
extern "C" int zls_arbno_zq(const IR_t *, int *, int);   /* s141 ARBNO-NOFILL: body ASSIGN_SAVE cell offsets (9 = overflow, keep eager fill) */
static int walk_bb_node_inner(IR_t * nd, FILE * out);
extern "C" int bbprof_on(void);
extern "C" void bbprof_record(int nid, int kind, int uid, void *lo, void *hi);
int g_zprobe_depth = 0;
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void emit_sep_rule(char ch) { if (emit_sep_on()) bb_emit_x86(x86("commentrule", std::string(119, ch))); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void emit_sep_rule_c(char ch) { emit_sep_rule(ch); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ZB-VAL-8b-USE-2 (s183): the use predicate is promoted HERE -- the ONE choke point every template invocation passes through (all three entry macros FILL / EMIT_PAIR_FILL / DRIVE_FILL end in walk_bb_node,
 * whose only two callees are this function).  s182 promoted it at the IR_CMP_TEST/IR_COERCE_NUMERIC dispatch arm instead, which was correct while bb_cmp_test was the sole reader but is STALE-UNSAFE the moment
 * a second consumer exists: DRIVE_FILL resets ~20 op_* fields and op_res_live is NOT among them, so a live node whose own arm never assigns it would inherit the PREVIOUS node's measured 0 and silently SKIP a
 * result store the reader needs -- wrong code, not slow code.  Keying it on nd at the choke point makes the value always the emitting node's own; zls_result_live is conservative (unknown node -> 1), so the
 * uniform promotion can only ever hand a template the same answer the arm-local promotion did, or a SAFER one.  This supersedes the s182 arm-local line, which is deleted (one authority, not two). */
static int zw_nid_listed(const char * e, int nid) { if (!e || !*e) return 0; const char * p = e; while (*p) { long v = strtol(p, (char **)&p, 10); if ((int)v == nid) return 1; while (*p && *p != ',') p++; if (*p) p++; } return 0; }   /* ZW-1: SCRIP_BB_ONLY/SKIP csv membership -- the per-BB arming instrument */
/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int g_zd_stage, g_zd_arm, g_zd_k, g_zd_gpop, g_zd_wpop, g_zd_read[6];   /* ZD-1 driver-handoff staging (R5b): the drive loop fills these from zd_plan's arrays immediately before emit_drive; the choke below applies them to g_emit exactly once and CLEARS the stage flag, so every other walk_bb_node entry path (FILL / EMIT_PAIR_FILL glue) sees zeros and stays byte-identical. */
/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int walk_bb_node_inner(IR_t * nd, FILE * out) {
    { extern int zls_result_live(const IR_t *); g_emit.op_res_live = zls_result_live(nd); }
    { static int _ba = -1; static const char * _bo; static const char * _bs; if (_ba < 0) { const char * e = getenv("SCRIP_BB_ALLOC"); _ba = (e && *e == '0') ? 0 : 1; _bo = getenv("SCRIP_BB_ONLY"); _bs = getenv("SCRIP_BB_SKIP"); }
      int _spine = (nd->op == IR_BINOP || nd->op == IR_ASSIGN || nd->op == IR_LIT_INTEGER || nd->op == IR_LIT_STRING || nd->op == IR_LIT_REAL || nd->op == IR_LIT_CHARSET || nd->op == IR_VAR || nd->op == IR_CMP_TEST || nd->op == IR_COERCE_NUMERIC);
      /* ⛔ ZTOS-1 NEGATIVE RESULT -- THE EXCLUSION IS EARNED, BUT NOT FOR THE REASON IT CLAIMED (measured s21x-o, do not retry blind).  Lon's directive was "turn on allocation for EVERY single BB, no need to be
       * shy", and the obvious reading was that this list is pure shyness: its own s21x-m note blamed "a zw carve-only cell inserted mid-spine displaces every TOS read by K", and ZTOS retires exactly that
       * displacement.  So the list was set to 0 and MEASURED: m3 214 -> 108, m4 211 -> 107.  The spine kinds themselves are FINE -- `OUTPUT = 1 + 2` still prints 3, and the A/B shows ZTOS compensating the
       * binop's reads correctly (+16 on every operand, the box's own carve).  What breaks is 111 programs that are almost all MATCH statements (033_goto_success and friends: the match runs, fails, and takes
       * the :F branch).  ROOT CAUSE, and it is the SAME root cause one level out: the match family does NOT address through ZTOS.  It speaks FRQ()/x86_zop arm 3-4, whose depth compensation is op_flat_disp --
       * LOWER's STATIC prefix sum, computed before any of this and blind to per-BB carves.  Arming a spine producer that FEEDS a match deepens rsp under a consumer whose compensation cannot see it.
       * THE LAW THIS ESTABLISHES: a BB may be armed once ALL of its consumers speak the live-depth authority (ZTOS/op_zdepth); arming it while a consumer still speaks the static authority (FRQ/op_flat_disp)
       * displaces that consumer by exactly the new carve.  So the arming frontier is not a property of the ARMED kind, it is a property of its READERS -- which is why widening by kind kept producing the
       * "unknown reasons" of s21x-j..m.  The ordered rung is therefore: migrate the match family's FRQ references onto the live authority FIRST, then this list retires by itself rather than by directive. */
      { static int _all = -1; if (_all < 0) { const char * e = getenv("SCRIP_BB_ALLOC_ALL"); _all = (e && *e == '1') ? 1 : 0; }   /* ZW-2 ALL-PORTS (Lon s21x-r: "there is no need to convert ANY BB -- the hook is in the ALPHA and BETA, just turn it on EVERYWHERE"): the universal carve already grants every node its own zw_node_k at alpha; the five kind exclusions below are the LAST hand-maintained list standing between it and every box. This flag drops them so ALL boxes self-allocate through the port hook, no per-template conversion. */
      (void)_ba; (void)_bo; (void)_bs; (void)_all; (void)_spine;
      { extern long zw_carve_k(const IR_t *); long _k = zw_carve_k(nd);   /* CARVE-ERAD step 1: the predicate that used to live here is now zw_carve_k (zeta_storage.c) so the LOWER-side prefix walk spends the SAME K this hook does -- one authority, per the zw_node_k precedent. */
        if (_k > 0) g_emit.op_fc_bytes = _k; } } }
    /* ZTOS-1 SLIDING-OFFSET AUTHORITY (Lon s21x-o "ONE main function that does graph traversal and calculates the zeta offsets"; "no pre-allocation calculation is necessary").  op_zdepth is that calculation,
     * and it is one line because the traversal that needs to happen is the one the emitter is ALREADY doing: walk_bb_node_inner is the single choke every template invocation passes through, and at this point
     * the node's own K is final.  The alpha carve about to be emitted (x86_port_hook's FORTH arm / bb_glue_flat_enter) subtracts exactly op_fc_bytes, so that IS the depth every TOS read inside this body must
     * add back.  Set unconditionally from the same field the allocator spends, so the allocator and the accessor CANNOT hold two opinions -- the ONE-K-AUTHORITY discipline (zw_node_k) extended to the address
     * side.  Zero when the box carves nothing, which is what makes every unconverted template byte-identical and lets the family-by-family conversion crawl safely. */
    g_emit.op_zdepth = x86_fc_on() ? (int)g_emit.op_fc_bytes : 0;   /* ZW-1 UNIVERSAL CARVE (Lon s21x-m "across the board, then crawl"): every previously-ungranted node self-allocates its whole zls extent at alpha and releases at every omega, THROUGH THE EXISTING x86_alpha/x86_omega HOOK (x86_asm.h ~1839 arms on op_fc_bytes alone) -- op_fc_base stays -1 so this is CARVE-ONLY: zero FR/FRQ spellings change, the one new machine effect is the rsp motion.  fc_geom-granted kinds keep their per-kind preamble geometry untouched (the !fc_geom conjunct); the FOUR-construct family (HEAD/FENCE1/SAVE_RESTORE/CALL, laws 4/6/7 -- self-managed windows and the frame dance) is excluded by kind; GOTO_DEFERRED is a control sink (abandoned depth by design).  SCRIP_BB_ONLY/SCRIP_BB_SKIP = the per-BB dynamic arming Lon asked for (nid csv), the crawl's bisect instrument. */
    g_emit.op_pair_rejoin = (nd && nd->op == IR_MATCH_ALTERNATE) ? 1 : 0;   /* ALT-NARY REJOIN (s22j): set at the ONE choke every template invocation passes through, keyed on IR KIND (the sanctioned emitter dispatch axis), so x86_asm.h consumes a promoted scalar and stays encodings-only.  ALTERNATE ONLY -- see the op_pair_rejoin comment in emit.h for the measured reason ARBNO must keep its per-iteration pair claims. */
    g_emit.op_zres = 0; g_emit.op_zgpop = 0; for (int _zi = 0; _zi < 6; _zi++) g_emit.op_zread[_zi] = 0;   /* ZD-1: cleared at the choke UNCONDITIONALLY so glue and non-drive fills can never inherit a stale regime */
    if (g_zd_stage) { if (g_zd_arm) { g_emit.op_zres = 1; g_emit.op_fc_bytes = g_zd_k; g_emit.op_fc_base = -1; g_emit.op_zdepth = g_zd_k; for (int _zi = 0; _zi < 6; _zi++) g_emit.op_zread[_zi] = g_zd_read[_zi]; }
        g_emit.op_zgpop = g_emit.flat_stmt_frame ? 0 : g_zd_gpop; g_emit.op_wpop += g_zd_wpop; g_zd_stage = 0; }   /* ZGPOP-STF (s22a): ZERO HAND-COUNTED POPS on an armed graph -- the seed's law (seed/test_sno_stmt_frame_1.s: "fail edge = bracket cut; ZERO hand-counted pops", and the RETURN floater's "mov rsp,rbp # reclaim EVERY statement/BB carve at any depth").  Under STF a cross-statement gamma edge is REDIRECTED to the statement head's st_x leave stub (emit.cpp ~2327), which emits bb_glue_framed_leave = mov rsp,rbp; pop rbp -- a DEPTH-INDEPENDENT cut that already reclaims every ZD cell the statement suspended.  The staged add rsp,K in front of it is therefore dead: witness 023-shape `OUTPUT = A + B`, where n7_assign emitted `add rsp,48` immediately before `jmp main_stγ` whose first instruction is `mov rsp,rbp`.  Zeroed HERE at the planner choke rather than at the x86_asm.h:1943 emission site so BOTH consumers agree -- the conditional-gamma path (x86_asm.h:282) routes through the jcc invert synth on `op_zgpop > 0`, and gating only the emitter would fire that synth with no payload.  This is the RBP half of law 4 doing the job the hand-counted pop was standing in for: the value spine rides RSP, the release rides the bracket. */   /* ZD-1 (Lon s21x-v): the planner's numbers land here, once, at the ONE choke every template invocation passes through.  op_fc_bytes = the planner's K OVERRIDES the zw extent (the alpha carve is exactly the result cell), base stays -1 (the hook's op_zres term supplies the suspended discipline), op_zdepth mirrors K for any residual legacy spelling inside a converted body, op_wpop ADDS the planner's under-cells term to whatever the trampoline chase already accumulated. */
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
    g_emit.op_sval = (nd->op == IR_VAR || nd->op == IR_VAR_REF || nd->op == IR_ASSIGN || nd->op == IR_LIT_STRING || nd->op == IR_LIT_CHARSET
                       || nd->op == IR_KEYWORD_ICON || nd->op == IR_KEYWORD_ICON_GEN || nd->op == IR_KEYWORD_SNOBOL4 || nd->op == IR_KEYWORD_ASSIGN || nd->op == IR_REV_SWAP || nd->op == IR_FIELD_GET || nd->op == IR_FIELD_VAR || nd->op == IR_SUBSCRIPT || nd->op == IR_ITERATE
                       || nd->op == IR_MATCH_ASSIGN_COND || nd->op == IR_MATCH_ASSIGN_SAVE || nd->op == IR_MATCH_ASSIGN_IMM || nd->op == IR_MATCH_LIT || nd->op == IR_MATCH_ANY || nd->op == IR_MATCH_NOTANY || nd->op == IR_MATCH_SPAN
                       || nd->op == IR_MATCH_BREAK || nd->op == IR_MATCH_BREAKX || nd->op == IR_MATCH_DEFER || nd->op == IR_MATCH_PATREF || nd->op == IR_MATCH_ATP || nd->op == IR_MATCH_REPLACE || nd->op == IR_GOTO_DEFERRED
                       || nd->op == IR_NULLTEST_VAR || nd->op == IR_PROC_GEN || nd->op == IR_PROC_VALUE || ir_norm_call_kind(nd->op) == IR_CALL)
                    ? IR_LIT(nd).sval : (const char *)0;
    { extern int g_gva_active; extern int gva_index_of(const char *); int nm_op = (nd->op == IR_VAR || nd->op == IR_VAR_REF || nd->op == IR_ASSIGN);
      int defer_op = ((nd->op == IR_MATCH_DEFER || nd->op == IR_MATCH_PATREF) && IR_LIT(nd).sval && IR_LIT(nd).sval[0] != '*');
      g_emit.op_gva_k = (g_gva_active && (nm_op || defer_op) && IR_LIT(nd).sval && !graph_has_local(g_emit_cfg, IR_LIT(nd).sval)) ? gva_index_of(IR_LIT(nd).sval) : -1; }
    { extern int g_proc_direct_active; extern int proc_slot_of(const char *); extern int proc_direct_eligible(const char *); int nm_op = (ir_norm_call_kind(nd->op) == IR_CALL);
      g_emit.op_proc_k = (g_proc_direct_active && nm_op && IR_LIT(nd).sval && proc_direct_eligible(IR_LIT(nd).sval)) ? proc_slot_of(IR_LIT(nd).sval) : -1; }
    g_emit.op_stno = (int32_t)IR_LIT(nd).ival;
    g_emit.op_ival = (ir_norm_call_kind(nd->op) == IR_CALL || nd->op == IR_PROC_GEN) ? (int64_t)nd->n_operands : IR_LIT(nd).ival;
    if (nd->op == IR_SAVE_RESTORE) { int64_t v = IR_LIT(nd).ival; int64_t r = (v == 1 || v == 2 || v == 3) ? v : 0; g_emit.op_ival = r; g_emit.op_sval = r ? (const char *)0 : IR_LIT(nd).sval; }   /* CALL2BB UNION-TAG: IR.h's sval/ival/dval are ONE union — roles 1-3 write ival and never sval; role 0 writes sval (the callee, an lp_strdup heap pointer) and never ival.  A heap pointer is never 1..3, so the arm read back IS the role; normalize HERE (the single dispatch point) so op_ival is a clean 0..3 and op_sval is name-or-null — templates never see the raw union.  The slice-1 code was accidentally correct (the pointer fell past the 1/2/3 role checks into the role-0 body); this makes the tag explicit and keeps the raw union read out of every other op_sval consumer. */
    g_emit.op_node_kind = (int)nd->op;
    g_emit.op_fb_rbp = g_emit.flat_fb_refine ? emit_fb_bit_of(nd) : 1;   /* FB-STMT per-node bit (Lon 2026-07-29) — twin lives in DRIVE_FILL */
    g_emit.op_dval = IR_LIT(nd).dval;
    g_emit.op_counter = 0;
    IR_t *op_a = (nd->n_operands > 0) ? nd->operands[0] : (IR_t *)0;
    g_emit.op_a_sval = op_a ? IR_LIT(op_a).sval : (const char *)0;
    g_emit.op_a_node_kind = op_a ? (int)ir_norm_call_kind(op_a->op) : -1;
    g_emit.op_a_slot = (op_a != (IR_t *)0) ? bb_slot_get(op_a) : -1;
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
    switch (nd->op) {
    case IR_LIT_INTEGER:
    case IR_LIT_STRING:
    case IR_LIT_CHARSET:
    case IR_LIT_REAL:               { { long fck; if (!g_emit.op_zres && fc_geom(nd, &fck)) { g_emit.op_fc_bytes = fck; g_emit.op_fc_base = g_emit.op_off; } } bb_emit_x86(bb_lit_scalar()); }         return 0;   /* ZB-VAL-0: registered-lit fixed-cell grant; ZD-1 nodes keep base=-1 (op_zres IS the discipline) */
    case IR_KEYWORD_ICON:
    case IR_KEYWORD_ICON_GEN:     bb_emit_x86(bb_keyword_icon());       return 0;
    case IR_KEYWORD_SNOBOL4:      bb_emit_x86(bb_keyword_snobol4());    return 0;
    case IR_KEYWORD_ASSIGN:       bb_emit_x86(bb_keyword_assign());     return 0;
    case IR_VAR:                  { extern int is_global(const char *);
        const char * _vn = IR_LIT(nd).sval;
        int _vn_reassignable_builtin = _vn && (!strcmp(_vn, "write") || !strcmp(_vn, "writes"));
        if (_vn && _vn[0] == '&') bb_emit_x86(bb_keyword_icon());
        else if (_vn && ((is_global(_vn) && !graph_has_local(g_emit_cfg, _vn)) || _vn_reassignable_builtin)) { { long fck; if (!g_emit.op_zres && fc_geom(nd, &fck)) { g_emit.op_fc_bytes = fck; g_emit.op_fc_base = g_emit.op_off; } } { extern long fc_vwpop(const IR_t *); long _w = fc_vwpop(nd); if (_w > 0 && !g_emit.op_zres) g_emit.op_wpop += (int)_w; } bb_emit_x86(bb_var_global()); }   /* ZB-VAL-3/5: registered global-read fixed-cell grant -- FRQ rebase + the invert+pop+jmp conditional-omega synth serve the template unchanged; wpop ADDS the under-cells release so a mid-tree NV fail restores rsp to statement entry */
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
    case IR_ASSIGN: {
        extern int is_global(const char *);
        { extern int fc_vread_fp(const IR_t *); g_emit.op_fc_disp = fc_vread_fp(nd); }   /* ZB-VAL-0: granted-lit operand read displacement (-1 = flat) */
        const char * _an = IR_LIT(nd).sval;
        int _an_reassignable_builtin = _an && (!strcmp(_an, "write") || !strcmp(_an, "writes"));
        { static int _fcd = -1; if (_fcd < 0) { const char *_e = getenv("SCRIP_FC_DIAG"); _fcd = (_e && _e[0] == '1') ? 1 : 0; }
          if (_fcd && !g_emit.op_zres && g_emit.op_fc_disp >= 0) fprintf(stderr, "[FC-ARM] kind=ASSIGN zres=%d fc_disp=%d stf=%d a_slot=%d off=%d fallback=%s\n", (int)g_emit.op_zres, g_emit.op_fc_disp, (int)g_emit.flat_stmt_frame, g_emit.op_a_slot, g_emit.op_off, (g_emit.op_a_slot >= 0 && g_emit.op_off >= 0) ? "FLAT-OK" : "NONE"); }   /* NON-POP census: same instrument as the BINOP site -- the assign FC arm's pop is additionally gated on !stf(), so stf is carried here */
        if (_an && ((is_global(_an) && !graph_has_local(g_emit_cfg, _an)) || _an_reassignable_builtin)) { bb_emit_x86(bb_assign_global()); return 0; }
        if (_an) { { static int _dd = -1; if (_dd < 0) { const char *_e = getenv("SCRIP_DRIVE_DIAG"); _dd = (_e && _e[0] == '1') ? 1 : 0; }
                     if (_dd && !(g_emit.op_sb >= 0 && g_emit.op_off >= 0 && (g_emit.op_a_slot >= 0 || g_emit.op_a_node_kind == (int)IR_OP_COUNT))) {
                         IR_t *_ra = (nd->n_operands > 0) ? nd->operands[0] : (IR_t *)0;
                         fprintf(stderr, "[ASSIGN-DIAG] var=%s sb=%d off=%d a_slot=%d a_kind=%d rhs_op=%s rhs_nops=%d\n", _an, g_emit.op_sb, g_emit.op_off, g_emit.op_a_slot, g_emit.op_a_node_kind, _ra ? bb_op_name(_ra->op) : "NULL", _ra ? _ra->n_operands : -1); } }
                   bb_emit_x86(bb_assign_local()); return 0; }
        fprintf(out, "# [walk_bb_node: kind=%d unhandled]\n", (int)nd->op); return 1;
    }
    case IR_BINOP_TEST:          bb_emit_x86(bb_binop_relop());       return 0;
    case IR_BINOP_RELOP_VAL:     bb_emit_x86(bb_binop_relop_val());   return 0;
    case IR_BINOP:
        { extern int fc_vbinop_active(const IR_t *); g_emit.op_fc_disp = fc_vbinop_active(nd) ? 0 : -1; }   /* ZB-VAL-1/5: registered value-spine binop -- operands ride rsp cells (rhs@0, lhs@16), result replaces both via one net add rsp,16; -1 = flat */
        { extern long fc_vwpop(const IR_t *); long _w = fc_vwpop(nd); if (_w > 0 && g_emit.op_fc_disp >= 0 && !g_emit.op_zres) g_emit.op_wpop += (int)_w; }   /* ZB-VAL-5: generic-path DT_FAIL omega must release ALL live statement cells (binop carves nothing at alpha, so wpop carries the whole depth) */
        { static int _fcd = -1; if (_fcd < 0) { const char *_e = getenv("SCRIP_FC_DIAG"); _fcd = (_e && _e[0] == '1') ? 1 : 0; }
          if (_fcd && !g_emit.op_zres && g_emit.op_fc_disp >= 0) fprintf(stderr, "[FC-ARM] kind=BINOP%d zres=%d fc_disp=%d sa=%d sb=%d off=%d fallback=%s\n", (int)g_emit.op_binop_kind, (int)g_emit.op_zres, g_emit.op_fc_disp, g_emit.op_sa, g_emit.op_sb, g_emit.op_off, (g_emit.op_sa >= 0 && g_emit.op_sb >= 0 && g_emit.op_off >= 0) ? "FLAT-OK" : "NONE"); }   /* NON-POP census: nodes still taking the Gen-1 FC consume arm, and whether the legacy flat arm could carry them if it were deleted */
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
    case IR_MATCH_LEN:            { bb_prepare(nd); bb_emit_x86(bb_match_len()); } return 0;   /* SN4-PAT-1 */
    case IR_MATCH_LIT:            { bb_prepare(nd); bb_emit_x86(bb_match_lit()); } return 0;   /* SN4-PAT-3 */
    case IR_MATCH_ANY:            { bb_prepare(nd); bb_emit_x86(bb_match_any()); } return 0;   /* SN4-PAT-3 */
    case IR_MATCH_NOTANY:         { bb_prepare(nd); bb_emit_x86(bb_match_notany()); } return 0; /* SN4-PAT-3 */
    case IR_MATCH_SPAN:           { bb_prepare(nd); { long fck; if (fc_geom(nd, &fck)) { g_emit.op_fc_bytes = fck; g_emit.op_fc_base = g_emit.x86_scratch_off; } } bb_emit_x86(bb_match_span()); } return 0;   /* SN4-PAT-3 + ZB-FC-0 fixed-cell grant */
    case IR_MATCH_BREAK:          { bb_prepare(nd); { long fck; if (fc_geom(nd, &fck)) { g_emit.op_fc_bytes = fck; g_emit.op_fc_base = g_emit.x86_scratch_off; } } bb_emit_x86(bb_match_break()); } return 0;   /* SN4-PAT-3 + ZB-FC-1 fixed-cell grant */
    case IR_MATCH_BREAKX:         { bb_prepare(nd); { long fck; if (fc_geom(nd, &fck)) { g_emit.op_fc_bytes = fck; g_emit.op_fc_base = g_emit.x86_scratch_off; } } bb_emit_x86(bb_match_breakx()); } return 0;  /* SN4-PAT-3 + ZB-FC-1 fixed-cell grant */
    case IR_MATCH_TAB:            { bb_prepare(nd); { long fck; if (fc_geom(nd, &fck)) { g_emit.op_fc_bytes = fck; g_emit.op_fc_base = g_emit.x86_scratch_off; } } bb_emit_x86(bb_match_tab()); } return 0;     /* SN4-PAT-3 + ZB-FC-1 fixed-cell grant */
    case IR_MATCH_RTAB:           { bb_prepare(nd); { long fck; if (fc_geom(nd, &fck)) { g_emit.op_fc_bytes = fck; g_emit.op_fc_base = g_emit.x86_scratch_off; } } bb_emit_x86(bb_match_rtab()); } return 0;    /* SN4-PAT-3 + ZB-FC-1 fixed-cell grant */
    case IR_MATCH_POS:            { bb_prepare(nd); bb_emit_x86(bb_match_pos()); } return 0;     /* SN4-PAT-3 */
    case IR_MATCH_RPOS:           { bb_prepare(nd); bb_emit_x86(bb_match_rpos()); } return 0;    /* SN4-PAT-3 */
    case IR_MATCH_REM:            { bb_prepare(nd); { long fck; if (fc_geom(nd, &fck)) { g_emit.op_fc_bytes = fck; g_emit.op_fc_base = g_emit.x86_scratch_off; } } bb_emit_x86(bb_match_rem()); } return 0;     /* SN4-PAT-3 + ZB-FC-2 fixed-cell grant */
    case IR_MATCH_ATP:            { bb_prepare(nd); bb_emit_x86(bb_match_atp()); } return 0;     /* SN4-PAT @ cursor-position capture */
    case IR_MATCH_ARB:            { bb_prepare(nd); { long fck; if (fc_geom(nd, &fck)) { g_emit.op_fc_bytes = fck; g_emit.op_fc_base = g_emit.x86_scratch_off; } } bb_emit_x86(bb_match_arb()); } return 0;     /* SN4-PAT-3 + ZB-FC-4 fixed-cell grant (ex-zls2, Lon s50 S14) */
    case IR_MATCH_BAL:            { bb_prepare(nd); { long fck; if (fc_geom(nd, &fck)) { g_emit.op_fc_bytes = fck; g_emit.op_fc_base = g_emit.x86_scratch_off; } } bb_emit_x86(bb_match_bal()); } return 0;                                                                                                            /* SN4-BAL (s34): generator over paren-balanced extents; no ZLS2 grant (in-frame, SPAN shape) */
    case IR_MATCH_DEFER: case IR_MATCH_PATREF: { bb_prepare(nd); g_emit.op_seal = nd->seal; if (nd->seal == 1) g_emit.op_off = drive_value_slot(nd); g_emit.op_defer_leaf_susp = g_emit_cfg ? fc_tail_defer_susp_g(g_emit_cfg, nd) : -1; bb_emit_x86(bb_match_defer()); } return 0;  /* SN4-PAT-FOLD deferred/stored pattern var; s137 OVER-SEAL: seal state written HERE (post-prologue, both drive paths) from IR_t.seal — op_off repointed at the defer.pad quad (the α rsp watermark) only when FULL-sealed (==1); s142: seal==2 = write-once class, entry-cell only, no pad; s153: op_defer_leaf_susp arms the ZERO-GUARDED β for priced tail-candidate leaves (the ε-resume phantom-pad convention) */
    case IR_MATCH_VALUE:          { bb_prepare(nd); bb_emit_x86(bb_match_value()); } return 0;  /* SN4 kill-manufactured-names: match operand[0]'s pattern value (op_a_slot), no name */
    case IR_MATCH_ARBNO:          { extern int fc_tail_arbno(const IR_t *, int *, int *, int *, int *); extern int fc_tail_ncap(const IR_t *); int _fpb = 0, _fpl = 0, _osb = 0, _hdr = 0;
                                    if (x86_zc_frame() == ZC_FRAME_RSP && fc_tail_arbno(nd, &_fpb, &_fpl, &_osb, &_hdr)) { g_emit.op_tail = 1; g_emit.op_sb = _osb; g_emit.op_sa = _hdr; g_emit.op_tail_fpb = _fpb; g_emit.op_tail_fpl = _fpl; g_emit.op_tail_ncap = fc_tail_ncap(nd); g_emit.op_tail_seal = (nd->n_operands > 3 && nd->operands[3] == nd) ? 1 : 0; g_emit.op_tail_dfr = fc_tail_dfr(nd); }
                                    else { int _mo = -1, _sp = 0; if (zls_arbno_geom(nd, &_mo, &_sp)) { g_emit.op_sa = _mo; int _chain = (x86_port_mode() == ZC_PORT_FORTH); g_emit.op_arbno_chain = _chain; g_emit.op_sb = (_sp + (_chain ? 24 : 16) + 15) & ~15; if (_chain) g_emit.op_arbno_nzq = zls_arbno_zq(nd, g_emit.op_arbno_zq, 8); } else { g_emit.op_sa = -1; g_emit.op_sb = -1; g_emit.op_arbno_chain = 0; } }
                                    /* PS-3 (s152) DEFER-TAIL candidacy (decided ENTIRELY here, emit drive time -- LOWER untouched, so a decline is byte-identical chain): body bracket must be EXACTLY one
                                     * WRITE-ONCE defer (seal==2, s142 class: wrcount==1 + fz-safe, so mid-match reassignment cannot exist) whose bound name is PROLOGUE-DOMINATED (assignment executes
                                     * before any label/goto ⇒ before any match ⇒ the slow defer path with its foreign frontier shape is unreachable) plus inert LIT_STRING/GOTO wiring, none of it
                                     * window-bearing; the target PAT$ must have registered a UNIFORM suspension footprint (the ζ size: align16(32+fb)+fp+16).  Fence-seal marker (operands[3]==nd)
                                     * declines v1.  SCRIP_ARBNO_LATCH=1 arms (opt-in this slice until monitor-proven); the chain staging above stays live -- the template selects dt first. */
                                    if (g_emit.op_arbno_chain && g_emit.op_off >= 0 && !(nd->n_operands > 3 && nd->operands[3] == nd) && g_emit_cfg && nd->n_operands >= 3) {
                                        static int _dtl = -1; if (_dtl < 0) { const char * _e = getenv("SCRIP_ARBNO_LATCH"); _dtl = _e ? (atoi(_e) != 0) : 0; }
                                        if (_dtl) {
                                            int _i0 = -1, _i1 = -1; for (int _j = 0; _j < g_emit_cfg->n; _j++) { if (g_emit_cfg->all[_j] == nd->operands[1]) _i0 = _j; if (g_emit_cfg->all[_j] == nd->operands[2]) _i1 = _j; }
                                            if (_i0 > _i1) { int _t = _i0; _i0 = _i1; _i1 = _t; }
                                            IR_t * _dn = NULL; int _ok = (_i0 >= 0 && _i1 >= 0);
                                            for (int _j = _i0; _ok && _j <= _i1; _j++) { IR_t * _x = g_emit_cfg->all[_j]; if (!_x) continue;
                                                if (_x->op == IR_MATCH_DEFER || _x->op == IR_MATCH_PATREF) { if (_dn || _x->seal != 2) _ok = 0; else _dn = _x; }
                                                else if (_x->op == IR_LIT_STRING || _x->op == IR_GOTO) { }   /* inert wiring: the fz procname literal + gotos; a granted quad on them is dead weight (never read per-iteration -- the SLOT-ELIDE census class), so fields do NOT decline (s152 fix: the op whitelist is the gate) */
                                                else _ok = 0; }
                                            const char * _pn = NULL;
                                            if (_ok && _dn) for (int _j = 0; _j < _dn->n_operands; _j++) { IR_t * _o = _dn->operands[_j]; if (_o && _o->op == IR_LIT_STRING && IR_LIT(_o).sval && !strncmp(IR_LIT(_o).sval, "PAT$", 4)) { _pn = IR_LIT(_o).sval; break; } }
                                            int _susp = 0;
                                            static int _pk = -1; if (_pk < 0) { const char * _e2 = getenv("SCRIP_ARBNO_PEEK"); _pk = _e2 ? (atoi(_e2) != 0) : 1; }   /* PS-3 SLICE 2 (s155): default-ON under the latch — the RECORD-CARRY arm needs no registry SUSP, so the uniformity gate and the recursion barrier drop from the license; =0 restores the s152 lookup-gated admission + [rsp+susp] template byte-verbatim (kill-switch law) */
                                            if (getenv("SCRIP_DT_DIAG")) fprintf(stderr, "[DT] arbno: ok=%d pn=%s var=%s pro=%d uni=%d pk=%d\n", _ok, _pn ? _pn : "-", (_dn && IR_LIT(_dn).sval) ? IR_LIT(_dn).sval : "-", (_dn && IR_LIT(_dn).sval) ? sno_name_prologue_bound(IR_LIT(_dn).sval) : -1, _pn ? emit_patzeta_lookup(_pn, &_susp) : -1, _pk);   /* PS-3 first-failing-gate diag (SCRIP_TAIL_DIAG precedent) */
                                            if (_ok && _dn && IR_LIT(_dn).sval && sno_name_prologue_bound(IR_LIT(_dn).sval) && (_pk ? 1 : (_pn && emit_patzeta_lookup(_pn, &_susp) != 0))) { g_emit.op_arbno_dt = 1; g_emit.op_arbno_dt_susp = _pk ? 0 : _susp; }   /* peek mode drops the PAT$-operand requirement too — the recursive *group defer carries none (the carry arm resolves the target through the s142 cell at match time, PAT$ identity never consulted) */   /* PS-3 s155: peek mode admits ANY prologue-bound seal==2 single-defer body (susp=0 selects the record-carry arm); legacy mode keeps the s152 uniform-SUSP form */
                                        }
                                    }
                                    bb_emit_x86(bb_match_arbno()); } return 0;   /* SN4-NARY-ARBNO: one node, flat-driven; R12-EXIT-1 carry-the-tail (rsp elements, op_sa = HDRB) or heap COLLECTION / ZB-FC-4 chain / PS-3 defer-tail; geometry staged here */
    case IR_MATCH_HEAD:           { extern int fc_tail_head(const IR_t *); int _th = (x86_zc_frame() == ZC_FRAME_RSP && fc_tail_head(nd)); if (fc_head_fp(nd) >= 0 || _th) { g_emit.op_fc_wbytes = 24; g_emit.op_fc_base = g_emit.op_off; } if (_th) g_emit.op_tail = 1; bb_emit_x86(bb_match_head()); } return 0;                  /* SN4-PAT-2 + ZB-FC-3d self-cell WINDOW + R12-EXIT-1 tail statements self-push too (bracket source) */
    case IR_MATCH_RELEASE:        { IR_t * _hd = nd->n_operands > 0 ? nd->operands[0] : (IR_t *)0; extern int fc_tail_release(const IR_t *, int *); int _br = -1;
                                    if (x86_zc_frame() == ZC_FRAME_RSP && _hd && fc_tail_release(_hd, &_br)) { g_emit.op_tail = 1; g_emit.op_fc_disp = _br; }
                                    else g_emit.op_fc_disp = _hd ? fc_head_fp(_hd) : -1;
                                    bb_emit_x86(bb_match_release()); } return 0;               /* BB-OWNED-ζ statement-scope pivot + ZB-FC-3d cross-box cell reads + R12-EXIT-1 tail: bracket read off the TOP ELEMENT then unwind */
    case IR_MATCH_REPLACE:        { bb_emit_x86(bb_match_replace()); } return 0;               /* SN4-REPL stages 4/5 splice */
    case IR_MATCH_ASSIGN_COND:    { g_emit.op_fc_disp = fc_cond_fp(nd); bb_emit_x86(bb_match_capture()); } return 0;               /* SN4-PAT-2 + ZB-FC-3c cross-box read grant */
    case IR_MATCH_ASSIGN_IMM:     { g_emit.op_fc_disp = fc_cond_fp(nd); bb_emit_x86(bb_match_capture()); } return 0;               /* $ immediate capture + ZB-FC-3c (identical mechanism, phase 2) */
    case IR_MATCH_ASSIGN_SAVE:    { { long fck; if (fc_geom(nd, &fck)) { g_emit.op_fc_bytes = fck; g_emit.op_fc_base = g_emit.op_off; } } bb_emit_x86(bb_match_capture()); } return 0;               /* SN4-PAT-3h phase-0 SAVE + ZB-FC-3c fixed-cell grant (δ at cell+0) */
    case IR_MATCH_ALTERNATE:      { bb_emit_x86(bb_match_alternate()); } return 0;                   /* SN4-PAT-3h ALT; ALT-FLAT s202 -- zero-cell box, address-dispatch template, no fc staging */
    case IR_MATCH_FENCE1:          { bb_emit_x86(bb_match_fence1()); } return 0;     /* SYNC-POINT ζ RELEASE sync point 2: NO fc_geom BY DESIGN — the watermark quad must stay [rbp+off] (depth-immune) because the σ glue reads it at the dynamic post-P depth */
    case IR_MATCH_ABORT:           { bb_emit_x86(bb_match_abort()); } return 0;      /* ABORT-NODE (s193): first live constructor is lower_snobol4.c TT_ABORT (was bare IR_GOTO); template pre-existed, flat_trivial_beta already anticipated the kind */
    case IR_MATCH_SEQUENCE:       { g_emit.op_fc_seq = (x86_port_mode() == ZC_PORT_FORTH && fc_seq_active(nd)) ? 1 : 0; g_emit.op_seq_static = g_seq_static_cur; { long fck; if (fc_geom(nd, &fck)) { g_emit.op_fc_bytes = fck; g_emit.op_fc_base = g_emit.op_off; } } bb_emit_x86(bb_match_sequence()); } return 0;   /* SN4-NARY-SEQ + ZB-FC-3b zero-LOCALS grant; SEQ-CELL (s21x-l): regime-keyed 16B own-cell grant, base = op_off (both FR refs are own-quad dwords), fc_geom's !fc_seq_active conjunct keeps chain-converted SEQs flat */
    case IR_SCAN_SEQUENCE:        { bb_emit_x86(bb_scan_sequence()); } return 0;   /* Icon scan concat: bb_match_sequence wiring, value = slice into own slot */
    case IR_SCAN_ALTERNATE:       { bb_emit_x86(bb_scan_alternate()); } return 0;  /* Icon scan alternation: bb_match_alternate wiring, value = slice into own slot */
    case IR_TO_BY:                { bb_prepare(nd); bb_emit_x86(bb_to_by()); } return 0;
    case IR_MAKE_LIST:            bb_emit_x86(bb_make_list());      return 0;
    case IR_CONJUNCTION:                 bb_emit_x86(bb_conjunction());           return 0;
    case IR_GOTO:                 { static int _mon = -1; if (_mon < 0) _mon = getenv("MONITOR_BIN") ? 1 : 0; extern void emit_mon_label_tap(int32_t); if (_mon && g_emit.op_stno > 0) emit_mon_label_tap(g_emit.op_stno); } bb_emit_x86(bb_goto());           return 0;
    case IR_GOTO_DEFERRED:             bb_emit_x86(bb_goto_dyn());       return 0;               /* EVAL/CODE runtime label transfer */
    case IR_SAVE_RESTORE:              { extern int fc_call_active(const IR_t *); if (g_emit.op_ival == 0 && nd->γ.node && fc_call_active(nd->γ.node) && nd->n_operands == 1 && nd->operands[0]) { extern int zls_off(const IR_t *); int _as = bb_slot_get(nd->operands[0]); if (_as < 0) _as = zls_off(nd->operands[0]); if (_as >= 0) { g_emit.op_fc_wbytes = 16; g_emit.op_fc_base = _as; } } bb_emit_x86(bb_save_restore()); } return 0;               /* SN4-FLAT-PROC linkage: roles normalized at the UNION-TAG block above (0 CALL2BB site / 1 RETURN / 2 FRETURN / 3 wire-adopt).  CALL2BB 3b: when the gamma-paired call is fc-registered, sr0 gets the ARG window (base = the one arg's quad) so its existing FRQB(slot, sb) staging read self-rebases to [rsp + sb] -- the arg CELL above sr0's own save block -- with ZERO sr0 template edits (the s21x-h "fc arm inside FRQB" window-readiness, now armed); driver-side neighbor resolution is the sanctioned gather-then-deposit pattern. */
    case IR_BOUND:                { g_emit.op_sb = 1; g_emit.op_off = zls_off(nd); g_emit.op_fc_bytes = 0; bb_emit_x86(bb_bound()); } return 0;   /* Op_Mark: save rsp at bounded-expression entry (interp.r Op_Mark) */
    case IR_UNMARK:               { IR_t * _mk = nd->n_operands > 0 ? nd->operands[0] : (IR_t *)0; g_emit.op_sb = 0; g_emit.op_off = _mk ? zls_off(_mk) : -1; g_emit.op_fc_bytes = 0; bb_emit_x86(bb_bound()); } return 0;   /* Op_Unmark: rsp = paired mark — discard abandoned retained-suspension carves (interp.r rsp=efp-1) */
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
    case IR_DISJUNCTION: { if (nd->n_operands > 0) { bb_emit_x86(bb_disjunction()); return 0; }   /* MOVE_LABEL-ERAD: Icon nary self-state form (operands = (entry,resume)×N + result×N, ival=N); the 0-operand shape is the legacy Prolog gate. IR_INDIRECT_GOTO case retired slice 3: zero producers anywhere (lower_if was the only one, now a committed DISJUNCTION); kind stays declared, drive_unowned aborts LOUD if one ever reappears */
                                                  bb_emit_x86(bb_indirect_goto()); } return 0;
    case IR_SCAN:                 { IR_t *_en = (nd->n_operands > 0) ? nd->operands[0] : NULL; IR_t *_bv = (nd->n_operands > 1) ? nd->operands[1] : NULL; g_emit.op_sb = 0; g_emit.op_off = nd_slot(_en); g_emit.op_sa = _bv ? nd_slot(_bv) : -1; g_emit.op_ival = zls_off(nd); g_emit.lbl_t0 = g_scan_body_beta ? g_scan_body_beta->name : NULL; g_emit.lbl_t0_p = g_scan_body_beta; bb_emit_x86(bb_gen_scan()); } return 0;
    case IR_SCAN_TAB:             { long fck; if (fc_geom(nd, &fck)) { g_emit.op_fc_bytes = fck; g_emit.op_fc_base = g_emit.op_off + 16; } bb_emit_x86(bb_scan_tab()); }    return 0;   /* ZB-ICN-FC-1: Icon's first FORTH cell — saved-cursor scratch at op_off+16 rides rsp; grant set HERE (post-DRIVE_FILL reset), result stays flat at op_off+0 */
    case IR_SCAN_MOVE:            { long fck; if (fc_geom(nd, &fck)) { g_emit.op_fc_bytes = fck; g_emit.op_fc_base = g_emit.op_off + 16; } bb_emit_x86(bb_scan_move()); }   return 0;   /* ZB-ICN-FC-2: move is tab's twin — saved-cursor scratch at op_off+16 rides rsp; grant set HERE (post-DRIVE_FILL reset) */
    case IR_SCAN_UPTO:            bb_emit_x86(bb_scan_upto());   return 0;
    case IR_SCAN_ANY:             bb_emit_x86(bb_scan_any());    return 0;
    case IR_SCAN_MANY:            bb_emit_x86(bb_scan_many());   return 0;
    case IR_SCAN_FIND:            bb_emit_x86(bb_scan_find());   return 0;
    case IR_SCAN_MATCH:           { long fck; if (fc_geom(nd, &fck)) { g_emit.op_fc_bytes = fck; g_emit.op_fc_base = g_emit.op_off + 16; } bb_emit_x86(bb_scan_match()); }  return 0;   /* ZB-ICN-FC-3: {0,1} match's needle scratch at op_off+16/+24 rides an rsp cell (transient alpha->gamma; single-yield keeps LIFO clean even in a scan) */
    case IR_SCAN_POS:             bb_emit_x86(bb_scan_pos());    return 0;
    case IR_SCAN_BAL:             bb_emit_x86(bb_scan_bal());    return 0;
    case IR_GLIT:                 { g_emit.op_name1 = IR_LIT(nd).sval; bb_emit_x86(bb_rk_glit()); } return 0;   /* RK-GRAM-3b grammar literal-match leaf */
    case IR_GCC:                  { g_emit.op_name1 = IR_LIT(nd).sval; bb_emit_x86(bb_rk_gcc());  } return 0;   /* RK-GRAM-3b grammar char-class leaf */
    case IR_RETURN: {
        IR_t *rv = (nd->n_operands > 0 && nd->operands[0]) ? nd->operands[0] : (IR_t *)0;
        g_emit.op_sa = rv ? bb_slot_get(rv) : -1; g_emit.op_dval = IR_LIT(nd).dval;
        /* GENP-SPINE (s92): `return e` from a generator delivers the value through the RETAINING γ epilogue (frame + resume record stay live on the spine), so a subsequent β resume MUST route to failure —
         * Icon semantics: resumption of a returned procedure fails.  Arm the poison: bb_return stores lbl_ω's address into the resume slot pre-γ, so res-landing dispatch jmp [rbp+slot] takes the ABSOLUTE
         * unwind + ω wire.  Explicit resets on the non-gen arm — op_sb/lbl_t1 persist in g_emit across boxes (IR_SUSPEND's drive writes them) and a stale pair would poison a det return's slot-less frame. */
        if (g_emit.flat_gen && g_suspend_resume_slot >= 0) { g_emit.op_sb = g_suspend_resume_slot; g_emit.lbl_t1_p = g_emit.flat_fail_p; g_emit.lbl_t1 = g_emit.flat_fail_p ? g_emit.flat_fail_p->name : (const char *)0; }
        else { g_emit.op_sb = -1; g_emit.lbl_t1_p = (bb_label_t *)0; g_emit.lbl_t1 = (const char *)0; }
        bb_emit_x86(bb_return()); return 0; }
    case IR_CALL_PROC_STAGED: case IR_CALL_BUILTIN: case IR_CALL_BUILTIN_GEN:
    case IR_CALL_BUILTIN_ICON: case IR_CALL_BUILTIN_SNOBOL4: case IR_CALL_BUILTIN_PROLOG:
    case IR_PROC_GEN:
    case IR_CALL: {
        g_emit.op_call_route = bb_call_route_classify(nd);
        { extern int fc_call_active(const IR_t *); extern long fc_vwpop(const IR_t *); extern int zls_off(const IR_t *); if (fc_call_active(nd)) { g_emit.op_fc_wbytes = 16; g_emit.op_fc_base = zls_off(nd); long _w = fc_vwpop(nd); if (_w > 0) g_emit.op_wpop += (int)_w; } }   /* CALL2BB 3b: registered value-spine call -- RESULT window only (base = the call's own zls quad = bcps_result_slot's answer, so the L(2) FRQ(off)/FRQ(off+8) stores rebase into the in-place cell), op_fc_wbytes NEVER op_fc_bytes: an alpha-sub hook would sit ON TOP of sr0's live block and shift the c2 landings' rsp_load64(16*k) restores by 16 (s21x-h probed fact).  wpop = the binop total-pop contract: post-L(2) omega live depth released wholesale. */
        bb_emit_x86(bb_call(nd));
        return 0;
    }
    case IR_FAIL:            bb_emit_x86(bb_fail());                            return 0;
    case IR_CUT:             bb_emit_x86(bb_cut());                             return 0;
    case IR_UNOP:
    case IR_UNOP_TEST: case IR_NULLTEST_VAR: {
        { extern int fc_vbinop_active(const IR_t *); g_emit.op_fc_disp = fc_vbinop_active(nd) ? 0 : -1; }   /* ZB-VAL-6b: registered value-spine unop -- its ONE operand rides the TOP cell [rsp+0..15] and that same cell becomes the result, so the box nets ZERO (no rsp instruction at all); -1 = flat */
        { extern long fc_vwpop(const IR_t *); long _w = fc_vwpop(nd); if (_w > 0 && g_emit.op_fc_disp >= 0 && !g_emit.op_zres) g_emit.op_wpop += (int)_w; }   /* ZB-VAL-5/6b: same total-release contract as the binop -- a unop carves nothing at alpha, so wpop carries the whole live depth */
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
    g_emit.op_fc_bytes = 0; g_emit.op_fc_base = -1; g_emit.x86_fc_synth = 240; g_emit.op_fc_fpmax = -1; g_emit.op_fc_seq = 0; g_emit.op_fc_disp = -1; g_emit.op_fc_wbytes = 0; g_emit.op_arbno_chain = 0; g_emit.op_arbno_nzq = 0; g_emit.op_tail = 0; g_emit.op_tail_fpb = 0; g_emit.op_tail_fpl = 0; g_emit.op_tail_seal = 0; g_emit.op_tail_ncap = 0; g_emit.op_arbno_dt = 0; g_emit.op_arbno_dt_susp = 0; g_emit.op_defer_leaf_susp = -1; g_emit.op_tail_dfr = 0; g_emit.op_flat_disp = 0; g_emit.op_fb_rbp = g_emit.flat_fb_refine ? emit_fb_bit_of(nd) : 1; \
    if (x86_zc_frame() == ZC_FRAME_RSP) { int _fld = fc_leaf_disp(nd); if (_fld != (int)0x80000000) g_emit.op_flat_disp = _fld; } \
    bb_label_t *_fs__=bb_label_fold((s)), *_ff__=bb_label_fold((f)); \
    g_emit.lbl_α=(a)->name; g_emit.lbl_γ=_fs__->name; g_emit.lbl_ω=_ff__->name; g_emit.lbl_β=(b)->name; \
    g_emit.lbl_α_p=(a); g_emit.lbl_γ_p=_fs__; g_emit.lbl_ω_p=_ff__; g_emit.lbl_β_p=(b); \
    walk_bb_node((nd), emit_outf()); } while(0)
#define DRIVE_PAIR_RESET()      do { g_emit.xa_bb_emit_pair_n = 0; } while(0)
#define DRIVE_PAIR_JMP(tgt)     do { int _i=g_emit.xa_bb_emit_pair_n++; g_emit.xa_bb_emit_pair_define[_i]=NULL; g_emit.xa_bb_emit_pair_jmp[_i]=(tgt); } while(0)
#define DRIVE_PAIR_DEF_JMP(l,t) do { int _i=g_emit.xa_bb_emit_pair_n++; g_emit.xa_bb_emit_pair_define[_i]=(l); g_emit.xa_bb_emit_pair_jmp[_i]=(t); } while(0)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void * rt_proc_get_fn(const char * name);
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" int fc_alt_fpmax(const IR_t *);
static int fc_alt_active(const IR_t *nd) { return nd && nd->op == IR_MATCH_ALTERNATE && x86_port_mode() == ZC_PORT_FORTH && fc_alt_fpmax(nd) >= 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" int fc_seq_active(const IR_t *);
static int fc_seq_on(const IR_t *nd) { return nd && x86_port_mode() == ZC_PORT_FORTH && fc_seq_active(nd); }
static int seq_static_on(const IR_t *nd) { return nd && nd->op == IR_MATCH_SEQUENCE; }   /* SPD SEQ-STATIC: sigma/phi static re-point is valid for EVERY match-sequence -- celled counter was pure glue-dispatch bookkeeping (counter==attempting-element invariant), FC eligibility only constrains the ZERO-CELL rsp math */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ZB-FC-3b: elem_of(source) -- the ALT arm_of() walk verbatim (largest j with pos(entry_j) <= pos(source) in
 * nodes[] order; elements are allocated contiguously after S and entry_j is each element's first-allocated
 * node, the s31 convention).  A converted SEQ re-points its inside edges STATICALLY: a sigma (inside-success)
 * from element i lands element i+1's ALPHA directly -- rsp is already at element i's yield frontier, exactly
 * where i+1's alpha expects it -- and the LAST element keeps na_s (now an empty glue that simply falls into
 * S.gamma).  A phi (inside-fail) from element i lands element i-1's BETA directly -- element i's subtree has
 * popped, so rsp is already at i-1's yield frontier -- and element 0 keeps na_f (empty, falls into S.omega).
 * ZERO rsp adjustment on either edge; the LIFO structure carries what seq_i used to carry. */
static int fc_seq_elem_of(IR_t **nodes, int n, int k, int src) {
    int N = (int)(nodes[k]->n_operands / 2), el = 0, sp = -1;
    for (int j = 0; j < N; j++) if (nodes[k]->operands[2 * j] == nodes[src] || nodes[k]->operands[2 * j + 1] == nodes[src]) return j;   /* SPD SEQ-STATIC: identity first -- source IS an element/resume root (the CLEAN-set guarantee); positional walk survives only as the FC fallback */
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
static void flat_drive_match_alt(IR_t **nodes, int n, int i, bb_label_t **lbls, bb_label_t **betas, bb_label_t **na_s, bb_label_t **na_f, bb_label_t ***fc_sig, bb_label_t *node_γ, bb_label_t *node_ω, bb_label_t *chain_ω) {
    /* SN4-NARY-ALT drive: operands = (entry_j, resume_j) pairs.  Pair slots handed to the template:
     * 0..N-1 = entry α labels (fail-glue's alt_i dispatch), N..2N-1 = resume β labels (A.β's alt_i dispatch
     * — the alternative's OWN inner resume, seed alt_β), 2N = na_s success-glue define, 2N+1 = na_f
     * fail-glue define.  σ/φ-tagged inside edges land the two glue defines (resolved in the chain loop). */
    IR_t *nd = nodes[i]; int N = (nd->op == IR_MATCH_ARBNO) ? 1 : (nd->op == IR_DISJUNCTION) ? (int)IR_LIT(nd).ival : nd->n_operands / 2;   /* ARBNO: (entry,resume) + [2]=geometry bracket + optional [3]=self seal marker; DISJUNCTION (MOVE_LABEL-ERAD): operands = (entry,resume)×N + result×N, so N rides ival, results at [2N..3N-1] */
    if (3 * N + 2 > XA_BB_EMIT_PAIR_MAX) { fprintf(stderr, "FATAL emit flat_drive_match_alt: node n%d op=%d N=%d needs %d pair slots > XA_BB_EMIT_PAIR_MAX(%d) — raise the constant in emit.h\n", i, (int)nd->op, N, 3 * N + 2, XA_BB_EMIT_PAIR_MAX); abort(); }
    g_emit.xa_bb_emit_pair_n = 0;
    for (int j = 0; j < N; j++) { IR_t *e = nd->operands[2 * j];     bb_label_t *t = node_ω; for (int k = 0; k < n; k++) if (nodes[k] == e) { t = lbls[k];  break; } if (nd->op == IR_DISJUNCTION && e && e->op == IR_FAIL && chain_ω) t = chain_ω;   /* MOVE_LABEL-ERAD: IR_FAIL is a chain SENTINEL (never emitted; edges into it = proc fail, the 1760 node_γ→lbl_ω convention) — a bare `fail` alternation arm must dispatch to the CHAIN ω, not the dj's statement-next ω (roman `integer(n)>0 | fail` bug: proc-fail was becoming statement-continue) */ int _i = g_emit.xa_bb_emit_pair_n++; g_emit.xa_bb_emit_pair_define[_i] = NULL; g_emit.xa_bb_emit_pair_jmp[_i] = t; }
    for (int j = 0; j < N; j++) { IR_t *r = nd->operands[2 * j + 1]; bb_label_t *t = node_ω; for (int k = 0; k < n; k++) if (nodes[k] == r) { t = betas[k]; break; } if (nd->op == IR_MATCH_ARBNO && nd->n_operands > 3 && nd->operands[3] == nd) t = na_f[i];   /* FENCE-rooted body: resume a committed iteration ≡ abandon it (seal) */ if (nd->op == IR_DISJUNCTION && r == nd) t = na_f[i];   /* MOVE_LABEL-ERAD self marker: valueless arm's resume ≡ advance to the next alternative (φ-glue) */ int _i = g_emit.xa_bb_emit_pair_n++; g_emit.xa_bb_emit_pair_define[_i] = NULL; g_emit.xa_bb_emit_pair_jmp[_i] = t; }
    { int _i = g_emit.xa_bb_emit_pair_n++; g_emit.xa_bb_emit_pair_define[_i] = na_s[i]; g_emit.xa_bb_emit_pair_jmp[_i] = NULL; }
    { int _i = g_emit.xa_bb_emit_pair_n++; g_emit.xa_bb_emit_pair_define[_i] = na_f[i]; g_emit.xa_bb_emit_pair_jmp[_i] = NULL; }
    /* ZB-FC-3a: per-arm sigma pad stubs (S10d pad-to-max).  Active iff LOWER registered the linear-arm fp set
     * AND the FORTH flavor is live; pairs 2N+2..3N+1 = stub defines, bodies emitted by the template.  The
     * fc_sig labels were allocated in the chain-body label loop; sigma edges into a granted ALT resolve to
     * stub[arm_of(source)] there.  Inactive => zero pairs pushed, zero routing, the pre-rung path verbatim. */
    if (nd->op == IR_MATCH_ALTERNATE && fc_sig && fc_sig[i])
        for (int j = 0; j < N; j++) { int _i = g_emit.xa_bb_emit_pair_n++; g_emit.xa_bb_emit_pair_define[_i] = fc_sig[i][j]; g_emit.xa_bb_emit_pair_jmp[_i] = NULL; }
    g_emit.op_off = drive_value_slot(nd); g_emit.op_ival = (int64_t)N;
    if (nd->op == IR_SCAN_SEQUENCE) { for (int j = 0; j < N && j < 32; j++) g_emit.op_parts_ival[j] = zls_off(nd->operands[2 * j + 1]); g_emit.op_parts_n = N; }   /* CV10: arm-value slots delivered to bb_scan_sequence (op_parts channel, IR_ALT precedent; 32-arm cap) */
    if (nd->op == IR_DISJUNCTION) { for (int j = 0; j < N && j < 32; j++) { IR_t *rj = (2 * N + j < nd->n_operands) ? nd->operands[2 * N + j] : (IR_t *)0; { int _g = 0; while (rj && rj->op == IR_CONJUNCTION && rj->n_operands > 0 && _g++ < 16) rj = rj->operands[0]; }   /* conjunction value = operands[0] (its own drive's alias source); chase STRUCTURALLY — bb_slot_register aliasing happens at the conj's OWN drive, later in chain order than this dj, so slot lookups here are pre-alias-stale (rung37 empty-value bug) */ int rs = rj ? emit_binop_opnd_slot(rj) : -1; g_emit.op_parts_ival[j] = rs; } g_emit.op_parts_n = N; }   /* MOVE_LABEL-ERAD option B: per-arm RESULT slots (trailing operands) for the σ-glue's alt_i-dispatched copy into the disjunction's own value slot; -1 = valueless arm, glue copies nothing (value stays as α zeroed it) */
    if (nd->op == IR_MATCH_SEQUENCE && x86_port_mode() != ZC_PORT_INSTRUMENTED && x86_port_mode() != ZC_PORT_ALLOC && (fc_seq_on(nd) || (seq_static_on(nd) && g_seq_static_cur))) { if (na_s[i]) bb_label_alias(na_s[i], bb_label_fold(node_γ)); if (na_f[i]) bb_label_alias(na_f[i], bb_label_fold(node_ω)); }   /* HEAT-1 H1b: FC/SEQ-STATIC arms emit as: jmp γ / af: jmp ω (bb_match_sequence.cpp) — pure trampolines; alias them so every later consumer jmp retargets to the final label (bb_label_fold at the jmp funnels, fixpoint chase, stubs still emitted for earlier consumers, SCRIP_SEQ_FOLD=0 kill-switch) */
    DRIVE_FILL(nd, lbls[i], node_γ, node_ω, betas[i]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void drive_arg_slots_reserve(int n) {
    if (n <= g_emit.op_arg_slot_cap) return;
    int cap = g_emit.op_arg_slot_cap > 0 ? g_emit.op_arg_slot_cap : 16;
    while (cap < n) cap *= 2;
    int *grown = (int *)realloc(g_emit.op_arg_slot, (size_t)cap * sizeof(int));
    if (!grown) { fprintf(stderr, "FATAL emit_drive: arg-slot staging realloc to %d failed\n", cap); abort(); }
    g_emit.op_arg_slot = grown; g_emit.op_arg_slot_cap = cap;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void drive_unowned(IR_t *nd) {
    fprintf(stderr, "FATAL emit_drive: IR op=%d has no template in the universal driver. Every op must be handled; the driver never declines silently. "
                    "Implement op=%d. NOTE: this is also the guard sink inside existing cases — if op=N plainly has a case, the BACKTRACE LINE "
                    "(not this message) names the failing guard.\n",
            nd ? (int)nd->op : -1, nd ? (int)nd->op : -1);
    abort();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void emit_drive(IR_t *nd, bb_label_t *lbl_α, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    if (!nd) { drive_unowned(nd); return; }
    /* ICN-SCAN-STRUCT: the scan-register-live flag is a STRUCTURAL property of each node (set at lower time
     * by TT_SCAN range-marking), not a function of emit-walk order. Publishing it here, before every node's
     * template runs, makes r13/r14/r15 sync decisions (bb_keyword_assign &subject/&pos, bb_suspend, bb_rev_swap,
     * x86_scan_sync_*) correct regardless of the four-port BFS order — the previous running counter dropped to 0
     * mid-body when a scan's IR_SCAN leave box was emitted before in-scan boxes reached via an IR_ACTIVATE fail edge. */
    { extern int g_scan_regs_live; g_scan_regs_live = nd->in_scan ? 1 : 0; }
    switch (nd->op) {
    case IR_LIT_STRING: case IR_LIT_INTEGER: case IR_LIT_REAL:
        g_emit.op_off = drive_value_slot(nd); DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    case IR_LIT_CHARSET:
        g_emit.op_sval = IR_LIT(nd).sval; g_emit.op_off = drive_value_slot(nd); DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    case IR_KEYWORD_ICON:
    case IR_KEYWORD_ICON_GEN:
    case IR_KEYWORD_SNOBOL4:
        g_emit.op_sval = IR_LIT(nd).sval; g_emit.op_off = drive_value_slot(nd); DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    case IR_KEYWORD_ASSIGN:
        g_emit.op_sval = IR_LIT(nd).sval; g_emit.op_off = drive_value_slot(nd); DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    case IR_VAR: {
        const char *vn = IR_LIT(nd).sval;
        if (vn && vn[0] == '&') { g_emit.op_sval = vn; g_emit.op_sa = -1; g_emit.op_off = drive_value_slot(nd); }
        else if (vn && is_global(vn) && !graph_has_local(g_emit_cfg, vn)) { g_emit.op_sa = -1; g_emit.op_off = drive_value_slot(nd); g_emit.op_sval = vn; g_emit.op_gva_k = g_gva_active ? gva_index_of(vn) : -1; }
        else if (vn) { int voff = bb_varslot_peek(vn); g_emit.op_sa = voff; g_emit.op_off = (voff >= 0) ? drive_value_slot(nd) : -1; }
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
          if (voff < 0) { fprintf(stderr, "[TE-4] IR_ASSIGN local '%s' has no LOWER-granted varslot — grant it in ir_drive_slot_assign (scrip_ir.c), never allocate in the emitter\n", vn); abort(); }
          g_emit.op_sb = voff; }
        g_emit.op_off = drive_value_slot(nd); DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_CALL: case IR_CALL_BUILTIN: case IR_CALL_BUILTIN_GEN: case IR_CALL_PROC_STAGED: case IR_PROC_GEN:
    case IR_CALL_BUILTIN_ICON: case IR_CALL_BUILTIN_SNOBOL4: case IR_CALL_BUILTIN_PROLOG: {
        int na = nd->n_operands; drive_arg_slots_reserve(na);
        for (int i = 0; i < na; i++) { IR_t * a = ir_call_arg(nd, i); g_emit.op_arg_slot[i] = nd_slot(a); }
        g_emit.op_arg_slot_n = na; g_emit.op_write_route = bb_call_write_route(nd);
        { extern int bb_scc_handoff_consume(const void *, const char *); g_emit.op_c2 = bb_scc_handoff_consume((const void *)nd, IR_LIT(nd).sval); }   /* CALL2BB: 1 = this exact node's sr0 armed the prefix; -1 = armed under a different callee name (bomb in the template); 0 = no partner */
        DRIVE_PAIR_RESET(); DRIVE_PAIR_DEF_JMP(lbl_β, lbl_ω); DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_MATCH_HEAD: {
        IR_t *subj = nd->n_operands > 0 ? nd->operands[0] : (IR_t *)0;
        g_emit.op_sa = subj ? drive_value_slot(subj) : -1;
        { extern int fc_vread_fp(const IR_t *); g_emit.op_subj_cell = (x86_zc_frame() == ZC_FRAME_RSP && x86_port_mode() == ZC_PORT_FORTH && subj && fc_vread_fp(nd) >= 0) ? 1 : 0; }   /* SUBJECT-CELL rung (a): the head consumes the subject DESCR from the FORTH cell the registered producer chain left at TOS; fvr is node-keyed so sharing the ASSIGN registry cannot collide (the line-866 consumer only ever sees IR_ASSIGN nodes) */
        g_emit.op_off = drive_value_slot(nd);
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_MATCH_RELEASE: {
        /* BB-OWNED-ζ statement-scope pivot: operand[0] = the statement's IR_MATCH_HEAD node (set by
         * sno_lower_match's ir_operand_push); read ITS slot, same convention as IR_MATCH_ARBNO's non-owner
         * phases reading role 0's slot via operand[0] two cases below. */
        IR_t *hd = nd->n_operands > 0 ? nd->operands[0] : (IR_t *)0;
        g_emit.op_off = hd ? drive_value_slot(hd) : -1;
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_GOTO_DEFERRED: {
        g_emit.op_sval = IR_LIT(nd).sval; DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_SAVE_RESTORE: {
        { int64_t v = IR_LIT(nd).ival; int64_t r = (v == 1 || v == 2 || v == 3) ? v : 0; g_emit.op_ival = r; g_emit.op_sval = r ? (const char *)0 : IR_LIT(nd).sval; }   /* UNION-TAG normalization, twin of the dispatch-point block (which runs later and would otherwise re-poison these from the raw union) */
        { extern void bb_scc_handoff_pending_set(const void *, const char *); extern void bb_scc_handoff_pending_clear(void);
          if (g_emit.op_ival == 0 && g_emit.op_sval && nd->γ.node) bb_scc_handoff_pending_set((const void *)nd->γ.node, g_emit.op_sval); else bb_scc_handoff_pending_clear(); }   /* CALL2BB: deposit the (call-node, callee) pair; the role-0 template ARMS it only if it emits the prefix */
        g_emit.op_c2 = 0;
        int na = nd->n_operands; drive_arg_slots_reserve(na);
        for (int i = 0; i < na; i++) { IR_t * a = nd->operands[i]; int s = a ? bb_slot_get(a) : -1; if (s < 0 && a) s = zls_off(a); g_emit.op_arg_slot[i] = s; }   /* CALL2BB slice 2: bcps_arg_slot's slot_get→zls precedence, -1 on total miss (role-0 declines to pass-through) */
        g_emit.op_arg_slot_n = na;
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;   /* SN4-FLAT-PROC: role rides ival (0 CALL2BB call-site save/install slice — sval = callee, op_arg_slot = staged arg slots, mirroring the call-family marshal; 1 RETURN floater, 2 FRETURN floater, 3 wire-adopt — roles 1-3 carry zero operands so the loop is inert for them) */
    }
    case IR_MATCH_REPLACE: {
        /* SN4-REPL: [0]=head (start@+0, RELEASE-stashed end@+24), [1]=repl value, [2]=subject value; sval=name */
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
    case IR_MATCH_ASSIGN_SAVE: {   /* SN4-PAT-3h phase-0: record the cursor into this node's own δ-slot */
        g_emit.op_off = drive_value_slot(nd); g_emit.op_phase = 0;
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_MATCH_FENCE1: {   /* s137 OVER-SEAL: ival==0 = the FENCE0 interior sync box — operand-free, PURE 4-port (α whacks to the rbp floor, β abandons), so it rides the GENERIC drive (op_ival promoted
                               * from IR_LIT at the prologue above; the granted quad is unused by the ival=0 template arm).  The operand form (FENCE(P), ival==1) keeps the flat-path-only contract below. */
        if (nd->n_operands == 0) { g_emit.op_off = drive_value_slot(nd); DRIVE_PAIR_RESET(); DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break; }
        drive_unowned(nd);       /* replaced below by flat_drive path — the n-ary constructs are driven from codegen_flat_chain_body where the label arrays live */
        break;
    }
    case IR_MATCH_ABORT: {   /* ABORT-NODE (s193): operand-free PURE 4-port kill box (α = jmp ω, β trampoline → ω) — labels only, no slot, so op_off = -1 (the POS no-operand sentinel) and no drive_value_slot grant, unlike FENCE0 above whose granted quad is at least reserved by its template contract. */
        g_emit.op_off = -1; DRIVE_PAIR_RESET(); DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_MATCH_ALTERNATE: case IR_MATCH_SEQUENCE: case IR_MATCH_ARBNO: case IR_SCAN_SEQUENCE: case IR_SCAN_ALTERNATE: {   /* SN4-NARY-ALT/-SEQ/-ARBNO + Icon scan nary: 2N operands = (entry_i, resume_i), N = n_operands/2 (ARBNO's [2] = geometry bracket, N=1); pairs 0..N-1 = entry α's, N..2N-1 = resume β's, 2N = success-glue def, 2N+1 = fail-glue def */
        drive_unowned(nd);       /* replaced below by flat_drive path — the n-ary constructs are driven from codegen_flat_chain_body where the label arrays live */
        break;
    }
    case IR_COERCE_STRING: case IR_COERCE_INTEGER: case IR_COERCE_REAL: {
        IR_t * a0 = nd->n_operands > 0 ? nd->operands[0] : (IR_t *)0;
        int sa = a0 ? emit_binop_opnd_slot(a0) : -1;
        if (sa < 0) { drive_unowned(nd); break; }
        g_emit.op_sa = sa; g_emit.op_off = drive_value_slot(nd);
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_COERCE_NUMERIC: case IR_CMP_TEST: {
        IR_t * a0 = nd->n_operands > 0 ? nd->operands[0] : (IR_t *)0;
        IR_t * a1 = nd->n_operands > 1 ? nd->operands[1] : (nd->op == IR_COERCE_NUMERIC ? a0 : (IR_t *)0);
        int sa = a0 ? emit_binop_opnd_slot(a0) : -1; int sb = a1 ? emit_binop_opnd_slot(a1) : -1;
        if (sa < 0 || sb < 0) { drive_unowned(nd); break; }
        g_emit.op_sa = sa; g_emit.op_sb = sb; g_emit.op_off = drive_value_slot(nd);
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_MATCH_LEN: {
        IR_t * a0 = nd->n_operands > 0 ? nd->operands[0] : (IR_t *)0;
        g_emit.op_sa = a0 ? bb_slot_get(a0) : -1; g_emit.op_off = -1;
        if (a0 && g_emit.op_sa < 0) { drive_unowned(nd); break; }
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
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_MATCH_BREAK: case IR_MATCH_BREAKX: {
        IR_t * a0 = nd->n_operands > 0 ? nd->operands[0] : (IR_t *)0;
        g_emit.op_sa = a0 ? bb_slot_get(a0) : -1;
        if (a0 && g_emit.op_sa < 0) { drive_unowned(nd); break; }
        g_emit.x86_scratch_off = drive_value_slot(nd);
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_MATCH_TAB: case IR_MATCH_RTAB: {
        IR_t * a0 = nd->n_operands > 0 ? nd->operands[0] : (IR_t *)0;
        g_emit.op_off = -1;
        if (!a0) { drive_unowned(nd); break; }
        if (a0->op == IR_LIT_INTEGER) { g_emit.op_sb = (int)IR_LIT(a0).ival; g_emit.op_sa = -1; }
        else { int sl = bb_slot_get(a0); if (sl < 0) { drive_unowned(nd); break; } g_emit.op_sa = sl; g_emit.op_sb = 0; }
        g_emit.x86_scratch_off = drive_value_slot(nd);
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_MATCH_POS: case IR_MATCH_RPOS: {
        IR_t * a0 = nd->n_operands > 0 ? nd->operands[0] : (IR_t *)0;
        g_emit.op_off = -1;
        if (!a0) { drive_unowned(nd); break; }
        if (a0->op == IR_LIT_INTEGER) { g_emit.op_sb = (int)IR_LIT(a0).ival; g_emit.op_sa = -1; }
        else { int sl = bb_slot_get(a0); if (sl < 0) { drive_unowned(nd); break; } g_emit.op_sa = sl; g_emit.op_sb = 0; }
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
    case IR_MATCH_DEFER: case IR_MATCH_PATREF: {
        g_emit.op_sa = -1; g_emit.op_off = -1;
        g_emit.x86_scratch_off = drive_value_slot(nd);
        g_emit.bb_child_fn = (void *) 0; g_emit.bb_child_lbl = (const char *) 0;
        /* ZS-2 (Lon s58): the FZ-5b frozen-head shortcut is RETIRED this rung — its AOT pattern procs are
         * call-regime proc_flat chains and the DEFER transfer is now jmp-entry, so frozen stored patterns
         * resolve through rt_defer_get_pat_fn like every other DT_P (the assignment still stores the recipe;
         * the blob compiles once and caches).  Restoring the shortcut under jmp-entry is PROC-CONV's business
         * (R12-FREE ladder rung 2), where the proc transfer itself converts.  The lower-side FZ-5b operand
         * mark (LIT_STRING at operands[0]) stays in the IR untouched for that rung to consume. */
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
          if (voff < 0) {
              fprintf(stderr, "[TE-4] IR_REV_ASSIGN local '%s' has no LOWER-granted varslot — grant it in ir_drive_slot_assign (scrip_ir.c), never allocate in the emitter\n", vn); abort();
          }
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
        if (ln[0] != '&') { int voff = bb_varslot_peek(ln); if (voff < 0) { fprintf(stderr, "[TE-4] IR_REV_SWAP lhs local '%s' has no LOWER-granted varslot — grant it in ir_drive_slot_assign (scrip_ir.c), never allocate in the emitter\n", ln); abort(); } g_emit.op_sb = voff; }
        if (rn[0] != '&') { int voff = bb_varslot_peek(rn); if (voff < 0) { fprintf(stderr, "[TE-4] IR_REV_SWAP rhs local '%s' has no LOWER-granted varslot — grant it in ir_drive_slot_assign (scrip_ir.c), never allocate in the emitter\n", rn); abort(); } g_emit.op_sa = voff; }
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
    case IR_DISJUNCTION: {   /* IR_INDIRECT_GOTO retired slice 3 (zero producers; Icon's if is a committed nary DISJUNCTION now) */
        if (nd->n_operands > 0) { drive_unowned(nd); break; }   /* MOVE_LABEL-ERAD: nary form is flat-driven from codegen_flat_chain_body like the other nary constructs */
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
        /* ICN-SCAN-STRUCT: removed g_scan_regs_live++ — the flag is now published per-node from nd->in_scan at emit_drive top (structural, order-independent). */
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_SCAN: {
        g_emit.op_sb  = 0;
        /* ICN-SCAN-STRUCT: removed g_scan_regs_live-- — see emit_drive top; leave/enter boxes carry in_scan=0 so the flag is 0 while their own templates run (they load/restore r13/r14/r15 unconditionally anyway). */
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
    default:
        drive_unowned(nd); break;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void emit_zeta_selfload(void) { int m = x86_port_mode(); if (m == ZC_PORT_INSTRUMENTED) { if (g_is_text) { char s[64]; snprintf(s, sizeof s, " test %s, %s\n jnz 1f\n ud2\n1:\n", x86_zr(), x86_zr()); emit_text_n(s, strlen(s)); } else { int z = x86_zr_num(), lo = z & 7; ef_b3((uint8_t)(0x48 | (z >= 8 ? 0x05 : 0x00)), 0x85, (uint8_t)(0xC0 | (lo << 3) | lo)); ef_b2(0x75, 0x02); ef_b2(0x0F, 0x0B); } } }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* BP-9 DEAD-β ELISION (s100): mark every chain node whose β label the emission loop below COULD reference; !used ⇒ the det leaf's β trampoline (define + jmp ω) is provably dead bytes and its template
 * elides it via x86_beta_trampoline().  This scan MIRRORS the loop's actual β consumers — γ/ω wires carrying the 2-byte β port tag (0xce 0xb2, GOTO chains chased through their γ wire exactly as the
 * loop does at its gamma_is_beta/omega_is_beta resolution), combinator arm resume (pair-loop operands of ALT/SEQ/ARBNO/SCAN/DISJUNCTION/REPALT), IR_MOVE_LABEL wantb targets, and the special-op class
 * whose betas feed resume globals (generators, SUSPEND, CALL/CALL_PROC_STAGED/CALL_BUILTIN_GEN/PROC_GEN, REPALT, LIMIT, GOTO, body_root).  ANY fc-converted SEQ/ALT in the chain ⇒ blanket all-used
 * (fc_seq_phi_tgt hands out PREV-ELEMENT betas positionally — slice-1 conservatism, revisit with the fc census).  Unproven ⇒ used; elision only ever deletes a label nothing can reach. */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* BP-9 (ii) TRIVIAL-β WHITELIST (LIVE CURSOR residual (a); the drift-hazard the gate script backs): an op is
 * listed IFF its template's ENTIRE β arm, under the conditions the drive case selects, is x86_beta_trampoline()
 * -- so its emitted β body is exactly [hook add rsp,K]* jmp <resolved-ω>, safe to inline at any ω-jmp site.
 * MIRRORED IN scripts/analyze_jump_redundancy.py --gate (verifies every listed op's emitted β bodies across the
 * bench corpus); a template edit that grows a real β body for a listed op FAILS that gate, not silently here.
 * IR_MATCH_ATP excluded (empty-sval arm returns an empty template).  IR_MATCH_ASSIGN_SAVE is trivial ONLY on
 * the fc-granted arm (ungranted = the rt_cap_push/pop path with a real β body), hence the fc_geom key. */
static int flat_trivial_beta(const IR_t *nd) {
    switch (nd->op) {
    case IR_MATCH_POS: case IR_MATCH_RPOS: case IR_MATCH_ABORT: return 1;
    case IR_MATCH_ASSIGN_SAVE: { long _k = 0; return fc_geom(nd, &_k) && _k > 0; }
    default: return 0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ZD-1 -- THE ONE MAIN FUNCTION (Lon s21x-v: "ONE main function that does graph traversal and calculates the zeta offsets based on the ORDER the BB's executed" + "NO FUNCTION-level processing whatsoever,
 * ONLY statement level scoping").  nodes[] IS the execution order the emitter lays down, so the traversal is a single forward pass over it, segmented into STATEMENTS at bb_src_of heads; depth resets to
 * zero at every head and never survives a statement, which is the whole of the scoping law.  Per statement the pass renders an ALL-OR-NOTHING VERDICT (the per-graph all-or-nothing law of s21x-t, one
 * level down): the statement rides ZD iff every node in it is either a whitelisted spine kind whose value operands all resolve to ZD producers inside the same statement, or a passive control node that
 * carves nothing -- one legacy reader or one foreign carve declines the whole statement, which is exactly the s21x-o reader-frontier law ("a BB may be armed once ALL of its consumers speak the live-depth
 * authority") enforced at the only granularity where it can be decided locally.  For an accepted statement the pass computes, in bytes: zout[i] = depth-on-exit (the running sum of alpha carves, 16 per
 * result cell, suspended through gamma per S10c); zgpop[i] = the statement-terminal gamma release (depth at exit when the gamma edge leaves the statement -- rsp returns to statement entry on EVERY
 * cross-statement success edge); zwpop[i] = the omega twin minus the box's own leave.  Consumers' operand displacements are then s21x-u's DIFFERENCE OF TWO DEPTHS, staged per node in the drive loop as
 * op_zread[k] = delta_at_read(C) - delta_out(P), the quantity no single depth can express.  SCRIP_ZD=0 kills the regime; SCRIP_ZD_ONLY/SCRIP_ZD_SKIP are the per-BB dynamic arming instruments (nid csv,
 * zw_nid_listed); SCRIP_ZD_DIAG=1 prints every verdict and every number this function computes. */
static IR_t * zd_chase(IR_t * t) { int g = 0; while (t && t->op == IR_GOTO && g++ < 128) t = t->γ.node; return t; }
/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int zd_wl_kind(IR_t * nd) {
    int op = (int)nd->op;
    extern int is_global(const char *);
    if (op == IR_LIT_INTEGER || op == IR_LIT_STRING || op == IR_LIT_REAL || op == IR_LIT_CHARSET) return 1;
    if (op == IR_UNOP) { int v = (int)IR_LIT(nd).ival; return (v == TT_MNS || v == TT_PLS || v == TT_SIZE || v == TT_CSET_COMPL) ? 1 : 0; }
    if (op == IR_BINOP) { long long o = (long long)IR_LIT(nd).ival; return (o == BINOP_ADD || o == BINOP_SUB || o == BINOP_MUL || o == BINOP_DIV || o == BINOP_MOD || o == BINOP_POW || o == BINOP_CUNION || o == BINOP_CDIFF || o == BINOP_CINTER || o == BINOP_CONCAT) ? 1 : 0; }   /* ZD-2a: BINOP_CONCAT ADMITTED -- bb_binop_concat_slot grew its ZD arm this rung, and binop_cat maps BINOP_CONCAT to BINOP_CAT_CONCAT which dispatches to that ONE template unconditionally (emit.cpp 654/917), so the 017 falsification below cannot recur for this op; RELOP/XREP stay out until 2b/2c give them arms.  ORIGINAL NOTE, still binding for the remaining categories: EXPLICIT arith/cset ops only -- exactly the set bb_binop_arith's ZD arm serves.  IR_BINOP also carries CONCAT/XREP/RELOP categories that dispatch to templates WITHOUT a ZD arm (emit.cpp binop_cat switch); arming those staged op_zres for a template that ignores it, so the legacy body read flat slots the producers never wrote -- the 017_concat_two_strings signature of the first wide run. */
    if (op == IR_VAR || op == IR_ASSIGN) { const char * vn = IR_LIT(nd).sval; return (vn && is_global(vn) && !graph_has_local(g_emit_cfg, vn)) ? 1 : 0; }   /* ⛔ ZD-2h RETIRED AS VACUOUS BY CONSTRUCTION -- DO NOT SPEND A RUNG WIDENING THIS LINE.  The carried NEXT item read "VAR-local/ASSIGN-local under the pin gate -- the depth-immune-base question, the C-style RBP flavor", i.e. arm locals only where rbp is pinned so a varslot read is not displaced by ZD depth.  MEASURED: the graph_has_local arm of this conjunct returns 0 for ZERO nodes across the whole crosscheck corpus, probed over every entry of nodes[] rather than inside the run loop (SCRIP_ZDLOCAL=1 below; the same probe flipped to the global arm fires 193 times in 40 programs, so the null is validated against a live control and is not an instrument artifact).  THE REASON IS SEMANTIC, NOT A CORPUS GAP: SNOBOL4 has no lexical locals.  A name in a DEFINE prototype's local list is an ORDINARY GLOBAL whose value is pushed on a stack at call, nulled, and restored on return (manual Ch.4 p.24 "any existing values ... will be saved on a pushdown stack ... restored to their previous values"; the formals get the identical treatment) -- which is law 7 of the s21x-c design of record, IR_SAVE_RESTORE handling them as globals in the one NV namespace.  graph_has_local is an ICON-era LEXICAL frame-slot concept (ARCH-ICON.md: Icon procedure params and local/static stay frame slots in both variable-model modes) and it has no SNOBOL4 customers at all.  There is no depth-immune-base question to answer here because there is no node class to answer it for.  The genuine RBP citizens of law 4 are the match family (ARBNO/FENCE1 variable-length housekeeping) and IR_CALL's frame dance -- that is where the "occasional RBP" of the design lives, and it is where the remaining declines are. */
    if (op == IR_COERCE_NUMERIC || op == IR_CMP_TEST) return 1;   /* ZD-2d/2e: both grew ZD arms this rung and both dispatch to ONE template unconditionally (emit.cpp case IR_COERCE_NUMERIC / IR_CMP_TEST).  They are ARMED AS A PAIR deliberately: CMP_TEST's operands are the COERCE_NUMERIC outputs, so arming either alone leaves the statement declining on the other and measures nothing -- the 65 and the 38 live in the SAME statements. */
    if (op == IR_GOTO_DEFERRED) return 1;   /* ZD-8 (a) */
    if (op == IR_KEYWORD_SNOBOL4) return 1;   /* ZD-2l: 13 first-blockers, dispatched to ONE template unconditionally (emit.cpp:880).  A ZERO-OPERAND leaf -- emit_chain_arity already classes it with the LIT_* producers at :2432 -- so zd_nops needs no arm and its default 0 is already right.  Admitted ALONE rather than with the rest of the tail: IR_SAVE_RESTORE carries a conditional g_emit-mutating preamble that is the CALL2BB arg-window linkage (:989), and IR_GOTO_DEFERRED is the EVAL/CODE runtime label transfer that flat_jmp_entry declines WHOLESALE by design -- both belong to the protocol rung, and cloning a value-spine arm onto either would be the 017 shape with extra steps. */
    if (op == IR_COERCE_STRING || op == IR_COERCE_INTEGER) return 1;   /* ZD-2k: the coerce tail, 17 + 7 first-blockers in this rung's census.  Both dispatch to ONE template unconditionally (emit.cpp 892/893, the two lines ABOVE the already-armed IR_COERCE_NUMERIC at 894) so the 017 falsification shape cannot recur, and both templates grew by-address ZD arms this rung.  IR_COERCE_REAL is deliberately NOT here: it shares case 1354's lowering group but its own dispatch was not audited and it censused ZERO first-blockers, so admitting it would arm an unexercised template for no measurable gain -- the ZD-2f vacuity lesson applied in advance. */
    if (op == IR_DEREF || op == IR_ASSIGN_VAR) return 1;   /* ZD-2j: the two kinds ZD-2f's null exposed as the REAL subscript gate (51 + 51 of its 106).  Both dispatch to ONE template unconditionally (emit.cpp 993/995) and both are by-value clones of the proven arith shape with a genuine DT_FAIL omega.  Armed TOGETHER with each other and WITH the already-written 2f subscript arm, which they unblock. */
    if (op == IR_FIELD_VAR) return 1;   /* ZD-2m: the LAST value-spine first-blocker, 5 of them, and with it the census is ALL PROTOCOL (CALL 518 / MATCH_HEAD 247 / SAVE_RESTORE 18 / GOTO_DEFERRED 6).  IR_FIELD_GET is DELIBERATELY NOT admitted beside it even though the two share one dispatch line (:1052) and one template: it is built ONLY by lower_icon.c:418 and lower_raku.c:445 and has ZERO SNOBOL4 customers, so admitting it would arm an unexercised kind for two frontends whose ladders have not validated ZD -- the ZD-2b ruling ("reachable only from another frontend, so it belongs to that language's ladder") applied to a kind that happens to be a co-tenant of this one's template rather than of its own.  The ZD arm's lv ternary still spells the FIELD_GET call target, so that ladder inherits a written arm when it arms the kind, exactly as ZD-2j inherited 2f's. */
    if (op == IR_SUBSCRIPT) return (nd->n_operands == 2) ? 1 : 0;   /* ZD-2f: the RVALUE x[i] form ONLY.  emit.cpp:992 routes 2-operand IR_SUBSCRIPT to bb_subscript (which grew a ZD arm) and every other arity to bb_section (which did NOT) -- arming by bare kind would hand a staged op_zres to a template that ignores it, the exact 017 falsification shape. */
    if (op == IR_CALL) { extern int rt_proc_is_registered(const char *); const char * fn = IR_LIT(nd).sval; return (fn && rt_proc_is_registered(fn)) ? 0 : 1; }   /* ZD-7 (a): bare IR_CALL admitted EXCEPT user-defined procs (DEFINE'd functions like add/roman/fib).  User procs route CALL_ROUTE_PROC_STAGED via bb_call_route_classify:752 which dispatches to bb_call_proc_staged_str -- that template has ZERO ZD arm; handing it op_zres=1 writes result via FRQ(resoff) with op_zdepth=K=16 added -> wrong flat-frame address.  MEASURED: 085/086/087 blank output until this exclusion added.  The op==IR_CALL_VALUE exclusion is implicit: IR_CALL_VALUE is a different op code, not reached here. */
    /* ZD-5 NOTE (s22i): IR_MATCH_HEAD is NOT admitted here yet.  The 247 "MATCH_HEAD declines" in the census
     * mean MATCH_HEAD was the FIRST blocker in the run -- but the ACTUAL blocker keeping those runs declined
     * is the subsequent pattern blob nodes (IR_MATCH_LIT, IR_MATCH_ALT, IR_MATCH_ASSIGN_SAVE, etc.) that have
     * no ZD arms.  Admitting MATCH_HEAD alone causes a segfault: the run partially arms (HEAD passes), then
     * falls into the pattern blob at a depth offset the flat FRQ reads don't expect.  Pattern blob ZD arming
     * is a future rung; MATCH_HEAD admission belongs on that rung, not here. */
    return 0;
}
/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int zd_nops(IR_t * nd) { int op = (int)nd->op; return (op == IR_UNOP || op == IR_ASSIGN || op == IR_DEREF || op == IR_COERCE_STRING || op == IR_COERCE_INTEGER || op == IR_FIELD_VAR) ? 1 : (op == IR_BINOP || op == IR_COERCE_NUMERIC || op == IR_CMP_TEST || op == IR_SUBSCRIPT || op == IR_ASSIGN_VAR) ? 2 : (op == IR_CALL) ? (int)nd->n_operands : 0; }   /* ZD-7 (b): IR_CALL n_operands = number of args passed; zd_plan's predecessor-in-run check iterates [0..nops) and stages op_zread[k] = delta_out(call) - delta_out(operand[k]) for each.  THIS IS THE ONE AUTHORITY -- the drive-loop staging site at emit.cpp:2346 calls it; adding IR_CALL here means the per-arg ZOPQ differences are staged automatically. */   /* ZD-2d/2e/2f: COERCE_NUMERIC reads operand 0 as the value and operand 1 as the partner type-peek; CMP_TEST takes both coerced operands by address; SUBSCRIPT takes base and index by value.  All stage two op_zread differences.  THIS IS THE ONE AUTHORITY -- the drive-loop staging site calls it rather than re-deriving, since the moment those two disagreed every operand rendered [rsp+0] and 29 programs silently returned wrong answers. */   /* ZD-7 (b): IR_CALL n_operands = number of args passed; zd_plan's predecessor-in-run check iterates [0..nops) and stages op_zread[k] = delta_out(call) - delta_out(operand[k]) for each.  THIS IS THE ONE AUTHORITY -- the drive-loop staging site at emit.cpp:2346 calls it; adding IR_CALL here means the per-arg ZOPQ differences are staged automatically. */   /* ZD-2d/2e/2f: COERCE_NUMERIC reads operand 0 as the value and operand 1 as the partner type-peek; CMP_TEST takes both coerced operands by address; SUBSCRIPT takes base and index by value.  All stage two op_zread differences.  THIS IS THE ONE AUTHORITY -- the drive-loop staging site calls it rather than re-deriving, since the moment those two disagreed every operand rendered [rsp+0] and 29 programs silently returned wrong answers. */
/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int zd_k(IR_t * nd) { int op = (int)nd->op; return (op == IR_ASSIGN || op == IR_GOTO_DEFERRED) ? 0 : 16; }   /* ZD-8 (b3) THIS IS THE ONE AUTHORITY for K, the alpha-carve byte count of an admitted node: a value-spine sink that yields no cell (IR_ASSIGN, IR_GOTO_DEFERRED) carves 0; every other admitted kind carves 16.  Until s22k this rule was SPELLED TWICE -- zd_plan's depth model and the drive loop's g_zd_k staging each held a copy -- and editing one alone made them disagree by exactly K, a silent stack skew on every node of the admitted kind (MEASURED: segfault, zero output, on 1020_code_label_transfer; it also cost s22j a misdiagnosis of the innocent by-name ZD arm).  Same disease zd_nops carries its own law for, and the s22i note at the staging site predicted this exact fold.  A kind whose K is neither 0 nor 16 changes THIS LINE ONLY. */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void zd_plan(IR_t **nodes, int n, unsigned char *zon, int *zout, int *zgpop, int *zwpop) {
    extern const char * bb_src_of(const IR_t *);
    static int _zd = -1, _dg = -1; static const char * _zo; static const char * _zs;
    if (_zd < 0) { const char * e = getenv("SCRIP_ZD"); _zd = (e && *e == '0') ? 0 : 1; const char * d = getenv("SCRIP_ZD_DIAG"); _dg = (d && *d == '1') ? 1 : 0; _zo = getenv("SCRIP_ZD_ONLY"); _zs = getenv("SCRIP_ZD_SKIP"); }
    for (int i = 0; i < n; i++) { zon[i] = 0; zout[i] = -1; zgpop[i] = 0; zwpop[i] = 0; }
    { static int _lp=-1; if(_lp<0){const char*e=getenv("SCRIP_ZDLOCAL");_lp=(e&&*e=='1')?1:0;} if(_lp){ extern int is_global(const char *); for (int i=0;i<n;i++){ int o=(int)nodes[i]->op; if(o==IR_VAR||o==IR_ASSIGN){ const char*vn=IR_LIT(nodes[i]).sval; int gl = (vn && is_global(vn) && !graph_has_local(g_emit_cfg, vn)); if(!gl) fprintf(stderr,"[ZDLOCAL] %s name=%s pinned=%d fbdata=%d\n", bb_op_name(nodes[i]->op), vn?vn:"<null>", x86_fb_pinned(), x86_fb_data()); } } } }
    if (!_zd || x86_port_mode() != ZC_PORT_FORTH || n <= 0 || g_emit.flat_jmp_entry) return;   /* ZD-1 JMP-ENTRY DECLINE (measured this session, the 1019_eval_string casualty -- the ONE real ZD break in the full ledger): runtime-compiled EVAL/CODE chains and PAT$/gen blobs are jmp-entry citizens that speak their OWN enter/exit protocol (32B wire header, value handoff through the protocol's slots -- the s193 discriminator comment at the emit_chain choke records the prior falsification "admitting ALL jmp-entry citizens regressed expr_eval/161/1016/1019").  A ZD-armed fragment spine lands its final value in a suspended CELL and the terminal gpop releases it, while the fragment protocol reads the slot it has always read -- garbage descriptor, m3 segfault at EVAL(P).  flat_jmp_entry is the named per-graph regime bit for exactly this citizenship; the outer main graph never sets it, so ZD lives everywhere it is proven and this family converts later on its own rung by teaching the protocol the cell convention. */
    int * claim = (int *)alloca(sizeof(int) * (size_t)n); int * run = (int *)alloca(sizeof(int) * (size_t)n); int * rpos = (int *)alloca(sizeof(int) * (size_t)n);
    for (int i = 0; i < n; i++) { claim[i] = -1; rpos[i] = -1; }
    /* ZD-1 REDESIGN (this session, the 033/058 falsification): statements are NOT contiguous ranges of nodes[] -- right-first lowering interleaves their members (058: stmt entry i=3, its consumer i=5,
     * ANOTHER stmt's entry i=4 between them), so range segmentation resolved operands across statements and staged releases on foreign producers.  Lon's words were "the ORDER the BB's EXECUTED": the
     * walk now follows the gamma wires.  Each bb_src_of head (plus the chain entry) roots a RUN: step cur = zd_chase(cur->gamma) -- GOTOs are chased exactly as the driver's own label resolution chases
     * them (dead nodes in emission, never members, never staged) -- until the stepped target is off-chain, a head, or already claimed.  The run IS the statement's executed spine, in execution order by
     * construction; membership, operand resolution, the depth walk, and the crossing classification all read it. */
    for (int hi = 0; hi < n; hi++) {
        if (!(hi == 0 || bb_src_of(nodes[hi]))) continue;
        if (claim[hi] >= 0) continue;
        int rl = 0; IR_t * cur = nodes[hi]; int guard = 0;
        while (cur && guard++ <= n) {
            int ci = -1; for (int k = 0; k < n; k++) if (nodes[k] == cur) { ci = k; break; }
            if (ci < 0 || claim[ci] >= 0) break;
            if (rl > 0 && bb_src_of(nodes[ci])) break;
            run[rl] = ci; rpos[ci] = rl; claim[ci] = hi; rl++;
            cur = zd_chase(cur->γ.node);
        }
        int ok = (rl > 0); const char * why = ""; int badi = -1;
        for (int r = 0; r < rl && ok; r++) { int i = run[r];
            if (!zd_wl_kind(nodes[i])) { ok = 0; why = bb_op_name(nodes[i]->op); badi = i; break; }
            if ((_zo && !zw_nid_listed(_zo, i)) || (_zs && zw_nid_listed(_zs, i))) { ok = 0; why = "nidgate"; badi = i; break; }
            { int no = zd_nops(nodes[i]); if (nodes[i]->n_operands < no) { ok = 0; why = "nops"; badi = i; break; }
              for (int j = 0; j < no; j++) { IR_t * p = nodes[i]->operands[j]; int f = -1; for (int k = 0; k < rl; k++) if (nodes[run[k]] == p) { f = k; break; }
                  if (f < 0 || f >= r) { ok = 0; why = "opnd"; badi = i; break; } } }
        }
        if (ok) { int zd = 0;
            for (int r = 0; r < rl; r++) { int i = run[r];
                int K = zd_k(nodes[i]);   /* ZD-8 (b1) sink -- K comes from zd_k, THE ONE AUTHORITY (defined beside zd_nops).  Do not re-spell the rule here. */
                zon[i] = 1; zout[i] = zd + K; zd = zout[i];
                IR_t * gt = zd_chase(nodes[i]->γ.node); IR_t * ot = zd_chase(nodes[i]->ω.node);
                int gin = 0; int oin = 0;
                for (int k = 0; k < rl; k++) { if (nodes[run[k]] == gt && k > r) gin = 1; if (nodes[run[k]] == ot && k > r) oin = 1; }
                if (!gin) zgpop[i] = zd;
                if (!oin) zwpop[i] = zd - K;   /* ZD-K2: was `zd - ((K > 0) ? 16 : 0)`, a THIRD spelling of the same rule -- it re-derived K from K and was correct ONLY because zd_k's range is exactly {0,16}.  It silently defeated zd_k's "a kind whose K is neither 0 nor 16 changes THIS LINE ONLY" promise: the first kind admitted at any other K would have had its omega twin subtract 16 while zout added K.  Provably byte-identical today (K==0 -> 0, K==16 -> 16), so this lands neutral and makes the one-authority real rather than coincidental. */
                if (_dg) fprintf(stderr, "[ZD] h=%d r=%d i=%d %s K=%d zout=%d gpop=%d wpop=%d\n", hi, r, i, bb_op_name(nodes[i]->op), K, zout[i], zgpop[i], zwpop[i]);
            } }
        else { for (int r = 0; r < rl; r++) rpos[run[r]] = -1;
               if (_dg && rl > 0) fprintf(stderr, "[ZD] run h=%d len=%d DECLINED at i=%d (%s op=%d)\n", hi, rl, badi, why, badi >= 0 ? (int)nodes[badi]->op : -1);
               { static int _cd = -1; if (_cd < 0) { const char *_e = getenv("SCRIP_CALL_DIAG"); _cd = (_e && _e[0] == '1') ? 1 : 0; }   /* ZD-7 TAG-SAFE CALLEE CENSUS: partition declined-run first blockers by kind; tag-safe: IR_CALL_VALUE excluded (no sval); IR_SAVE_RESTORE excluded (ival=role, not a pointer); all other ir_norm_call_kind==IR_CALL sub-kinds write sval in every lowerer. */
                 if (_cd && rl > 0 && badi >= 0) { IR_t * _bn = nodes[badi]; int _bop = (int)_bn->op; int _is_call = (_bop == (int)IR_CALL || (_bop != (int)IR_CALL_VALUE && ir_is_call_kind((IR_e)_bop))); const char * _callee = (_is_call && IR_LIT(_bn).sval) ? IR_LIT(_bn).sval : (const char *)0; fprintf(stderr, "[CALL-DIAG] DECLINE blocker op=%s(%d) callee=%s rl=%d badi=%d narg=%d\n", bb_op_name(_bn->op), _bop, _callee ? _callee : "-", rl, badi, _is_call ? (int)_bn->n_operands : -1); } } }   /* ZD-7 RUN-LENGTH PROBE: rl=run length, badi=blocker position in run (0 = head), narg=n_operands for call kinds -- proves whether IR_CALL runs are always single-node (rl=1, badi=0) or whether blockers appear mid-run (badi>0, meaning ZD-armed predecessors exist in the same run and must be read via ZOPQ rather than flat slots). */
    }
}
/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void flat_beta_used_scan(IR_t **nodes, int n, unsigned char *used) {
    for (int j = 0; j < n; j++) if (fc_seq_on(nodes[j]) || fc_alt_active(nodes[j])) { for (int k = 0; k < n; k++) used[k] = 1; return; }
    for (int k = 0; k < n; k++) {
        int op = (int)nodes[k]->op;
        used[k] = (ir_is_generator_kind(nodes[k]->op) || op == IR_SUSPEND || op == IR_CALL || op == IR_CALL_PROC_STAGED || op == IR_CALL_BUILTIN_GEN || op == IR_PROC_GEN || op == IR_REPALT
                   || op == IR_LIMIT || op == IR_GOTO || (g_emit_cfg && nodes[k] == g_emit_cfg->body_root)) ? 1 : 0;
    }
    for (int j = 0; j < n; j++) {
        int op = (int)nodes[j]->op;
        if (op == IR_MATCH_ALTERNATE || op == IR_MATCH_SEQUENCE || op == IR_MATCH_ARBNO || op == IR_MATCH_FENCE1 || op == IR_SCAN_SEQUENCE || op == IR_SCAN_ALTERNATE || op == IR_DISJUNCTION || op == IR_REPALT)
            for (int a = 0; a < nodes[j]->n_operands; a++) for (int k = 0; k < n; k++) if (nodes[k] == nodes[j]->operands[a]) used[k] = 1;
        if (op == IR_MOVE_LABEL && nodes[j]->n_operands > 0 && nodes[j]->operands[0] && (int)IR_LIT(nodes[j]).ival)
            for (int k = 0; k < n; k++) if (nodes[k] == nodes[j]->operands[0]) used[k] = 1;
        int gib = (nodes[j]->γ.sz[0] == (char)0xce && (unsigned char)nodes[j]->γ.sz[1] == 0xb2);
        int oib = (nodes[j]->ω.sz[0] == (char)0xce && (unsigned char)nodes[j]->ω.sz[1] == 0xb2);
        IR_t *gtgt = nodes[j]->γ.node;
        IR_t *otgt = nodes[j]->ω.node;
        { int _gg = 0; IR_t * _g = gtgt; while (_g && _g->op == IR_GOTO && _gg++ < 128) { if (!gib) gib = (_g->γ.sz[0] == (char)0xce && (unsigned char)_g->γ.sz[1] == 0xb2); gtgt = _g->γ.node; _g = gtgt; } }
        { int _gg = 0; IR_t * _o = otgt; while (_o && _o->op == IR_GOTO && _gg++ < 128) { if (!oib) oib = (_o->γ.sz[0] == (char)0xce && (unsigned char)_o->γ.sz[1] == 0xb2); otgt = _o->γ.node; _o = otgt; } }
        if (gib) for (int k = 0; k < n; k++) if (nodes[k] == gtgt) used[k] = 1;
        if (oib) for (int k = 0; k < n; k++) if (nodes[k] == otgt) used[k] = 1;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int codegen_flat_chain_body(IR_t *entry, const char *prefix) {
    bb_label_t lbl_α, lbl_α_body, lbl_γ, lbl_ω, lbl_β, lbl_res;
    emit_label_initf(&lbl_α,      "%s_α",      prefix);
    emit_label_initf(&lbl_α_body, "%s_α_body", prefix);
    emit_label_initf(&lbl_γ,       "%s_γ",      prefix);
    emit_label_initf(&lbl_ω,       "%s_ω",      prefix);
    emit_label_initf(&lbl_β,       "%s_β",       prefix);
    emit_label_initf(&lbl_res,     "%s_res",     prefix);
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
    { int guard = 0; while (entry && (entry->op == IR_SUCCEED || entry->op == IR_FAIL) && entry->γ.node && guard++ < CH_MAX) entry = entry->γ.node; }
    /* PROC-CONV: an EMPTY body (bare `:(RETURN)` / `:(FRETURN)` — the terminal is a childless SUCCEED/FAIL, so
     * the BFS below collects ZERO nodes) must not let lbl_α_body FALL THROUGH into the jmp-entry lbl_res landing
     * (`add rsp,8; pop zr`), which corrupts rsp/zr on the initial α pass — res is reachable ONLY via the outside
     * `jmp qword [rsp+0]` backtrack.  A non-empty body's last node always JMPs to lbl_γ/lbl_ω (both defined AFTER
     * res), so it never falls in; only n==0 does.  Record the terminal so α_body can jump straight to it. */
    int flat_empty_body_fail = (entry && entry->op == IR_FAIL) ? 1 : 0;
    int flat_empty_body_succ = (entry && entry->op == IR_SUCCEED) ? 1 : 0;
    entry = entry;
    /* RPO-FILL v2 (s22a): TRUE reverse-post-order.  v1 (s21x-z) named itself RPO but implemented PREORDER
     * (append on first visit), which does NOT guarantee a node precedes its successors: for A->B, A->C,
     * B->D, C->D preorder yields A,B,D,C -- D emitted before its predecessor C.  Nothing broke only because
     * the arithmetic shapes carry no merges (coverage, not correctness).  This walk is the real thing, in the
     * jcon_irgen.icn:472 ir_a_Binop shape: a node's successors are FINISHED before the node itself is emitted
     * (children before parent = post-order), then the post-order vector is REVERSED into nodes[], which is
     * what makes every node precede all of its successors.
     * Two-phase iterative DFS (the standard explicit-stack post-order): each node is pushed twice -- once
     * TAGGED_EXPAND (discover + push successors), once TAGGED_EMIT (append to post-order).  Because the stack
     * is LIFO and the EMIT entry is pushed BEFORE the successors, every successor subtree drains before the
     * node's own EMIT pops.  Successor push order is INVERTED relative to v1: γ is pushed FIRST so it pops
     * LAST, hence finishes last, hence -- after the reversal -- lands IMMEDIATELY after its predecessor.
     * That is the fall-through/contiguity property v1 got by accident and this gets by construction.
     * The visited set is now its own array (`seen`), not nodes[]: a node is marked at EXPAND time but does
     * not enter nodes[] until EMIT, so nodes[] is no longer a usable membership test mid-walk.
     * Two-pass structure preserved: pass 1 = entry + enterable-chain roots; pass 2 = generator ω tails. */
    static IR_t *postv[CH_MAX]; int pn = 0;
    static const IR_t *seen[CH_MAX]; int sn = 0;
#define RPO_VISITED(p) ({ int _v = 0; for (int _vi = 0; _vi < sn; _vi++) if (seen[_vi] == (p)) { _v = 1; break; } _v; })
#define RPO_MARK(p)    do { if (sn < CH_MAX) seen[sn++] = (const IR_t *)(p); } while (0)
#define RPO_TAG_EMIT   ((IR_t *)1)
#define RPO_PUSH(p)    do { if ((p) && (p)->op != IR_SUCCEED && (p)->op != IR_FAIL && !RPO_VISITED(p) && qt < Q_MAX) queue[qt++] = (IR_t *)(p); } while (0)
#define RPO_PUSH_SUCCS(c) \
        /* γ FIRST = popped LAST = finishes last = immediately follows (c) after the reversal */ \
        RPO_PUSH((c)->γ.node); \
        if (((c)->op == IR_MATCH_ALTERNATE || (c)->op == IR_MATCH_SEQUENCE || (c)->op == IR_MATCH_ARBNO || (c)->op == IR_MATCH_FENCE1 || (c)->op == IR_SCAN_SEQUENCE || (c)->op == IR_SCAN_ALTERNATE || (c)->op == IR_DISJUNCTION) && (c)->n_operands > 0) for (int _oi = (c)->n_operands - 1; _oi >= 0; _oi--) RPO_PUSH((c)->operands[_oi]); \
        if (((c)->op >= IR_MATCH_LIT && (c)->op <= IR_MATCH_ASSIGN_SAVE) || (c)->op == IR_MATCH_DEFER || (c)->op == IR_MATCH_PATREF || (c)->op == IR_MATCH_VALUE) RPO_PUSH((c)->ω.node); \
        if ((c)->op == IR_REPALT && (c)->n_operands > 1) RPO_PUSH((c)->operands[1]); \
        if ((c)->op == IR_REPALT && (c)->n_operands > 0) RPO_PUSH((c)->operands[0]); \
        if ((c)->op == IR_MOVE_LABEL && (c)->n_operands > 1) RPO_PUSH((c)->operands[1]); \
        if ((c)->op == IR_MOVE_LABEL && (c)->n_operands > 0) RPO_PUSH((c)->operands[0]); \
        if ((c)->op == IR_CREATE && (c)->n_operands > 0) RPO_PUSH((c)->operands[0]); \
        if ((c)->op == IR_SUSPEND && (c)->n_operands > 1) RPO_PUSH((c)->operands[1]); \
        if (((c)->op == IR_SUBSCRIPT || (c)->op == IR_RANDOM || (c)->op == IR_DEREF || (c)->op == IR_ASSIGN_VAR || (c)->op == IR_REV_ASSIGN_VAR || (c)->op == IR_KEYWORD_ASSIGN || (c)->op == IR_SCAN_TAB || (c)->op == IR_SCAN_MOVE || (c)->op == IR_SCAN_POS || (c)->op == IR_SCAN_MATCH || (c)->op == IR_SCAN_ANY || (c)->op == IR_SWAP_VAR || (c)->op == IR_CALL_VALUE || (c)->op == IR_VAR) && (c)->ω.node) RPO_PUSH((c)->ω.node); \
        if (((c)->op == IR_CALL || ir_is_call_kind((c)->op) || (c)->op == IR_PROC_GEN || (c)->op == IR_ACTIVATE) && (c)->ω.node) RPO_PUSH((c)->ω.node); \
        if (((c)->op == IR_BINOP || (c)->op == IR_BINOP_TEST || (c)->op == IR_BINOP_RELOP_VAL || (c)->op == IR_UNOP || (c)->op == IR_UNOP_TEST || (c)->op == IR_NULLTEST_VAR || (c)->op == IR_COERCE_STRING || (c)->op == IR_COERCE_INTEGER || (c)->op == IR_COERCE_NUMERIC || (c)->op == IR_COERCE_REAL || (c)->op == IR_CMP_TEST) && (c)->ω.node) RPO_PUSH((c)->ω.node); \
        (void)0;
    /* THE ONE WALK, run twice.  Drains the stack in two-phase post-order: an EXPAND pop discovers the node,
     * re-pushes it tagged EMIT, then pushes its successors ON TOP -- so the successors drain first and the
     * EMIT pop appends the node only after every successor subtree is already in postv[]. */
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
    /* THE REVERSAL, PER ROOT-BLOCK -- post-order becomes REVERSE-post-order.  This is the step v1 never had.
     * ⛔ REVERSING THE WHOLE VECTOR IS WRONG, AND IT COSTS THE ENTRY NODE: RPO_PUSH's visited test fires at
     * EXPAND time, so if a later root's subtree reaches `entry` first, `entry` is absorbed INTO that root's
     * post-order block and no longer finishes last -- it stops being nodes[0], the prologue falls into the
     * wrong statement, and the program starts executing in the middle (measured: 214_indirect_goto printed
     * "at BETA" forever, never "at ALPHA").  So each ROOT gets its own drain and its own reversal, and the
     * root blocks are appended in v1's order: entry, then anchors ascending, then the generator ω tails.
     * Block ORDER is therefore byte-identical to v1/BFS; only the order WITHIN a block changes. */
#define RPO_FLUSH() do { for (int _i = pn - 1; _i >= 0; _i--) { if (n >= CH_MAX) { fprintf(stderr, "[GZ-7] FATAL chain exceeds CH_MAX\n"); abort(); } nodes[n++] = postv[_i]; } pn = 0; } while (0)
    /* pass 1: entry + enterable-chain roots (SN4-FLAT-PROC s176: DEFINE bodies, RETURN/FRETURN floaters) */
    RPO_PUSH(entry); RPO_DRAIN(); RPO_FLUSH();
    { extern int zls_g_group_count(const IR_graph_t *); extern const IR_t * zls_g_group_anchor(const IR_graph_t *, int);
      if (g_emit_cfg && (!g_is_text || entry == g_emit_cfg->entry)) { int _gc = zls_g_group_count(g_emit_cfg); for (int _k = 0; _k < _gc; _k++) { const IR_t * _a = zls_g_group_anchor(g_emit_cfg, _k); if (_a) { RPO_PUSH(_a); RPO_DRAIN(); RPO_FLUSH(); } } } }
    /* pass 2: generator ω tails not reached by pass 1 (same logic as prior second BFS pass) */
    for (int i = 0; i < n; i++) if (ir_is_generator_kind(nodes[i]->op) && nodes[i]->ω.node && !RPO_VISITED(nodes[i]->ω.node)) RPO_PUSH(nodes[i]->ω.node);
    RPO_DRAIN(); RPO_FLUSH();
#undef RPO_FLUSH
#undef RPO_DRAIN
#undef RPO_PUSH_SUCCS
#undef RPO_PUSH
#undef RPO_TAG_EMIT
#undef RPO_MARK
#undef RPO_VISITED
    if (qt >= Q_MAX) { fprintf(stderr, "[GZ-7] FATAL: chain traversal queue saturated (qt=%d >= Q_MAX=%d) for prefix=%s -- control-flow edges were silently dropped; raise CH_MAX\n", qt, (int)Q_MAX, prefix); abort(); }
    unsigned char *zd_on = (unsigned char *)alloca((size_t)(n > 0 ? n : 1)); int *zd_out = (int *)alloca(sizeof(int) * (size_t)(n > 0 ? n : 1)); int *zd_gp = (int *)alloca(sizeof(int) * (size_t)(n > 0 ? n : 1)); int *zd_wp = (int *)alloca(sizeof(int) * (size_t)(n > 0 ? n : 1));
    zd_plan(nodes, n, zd_on, zd_out, zd_gp, zd_wp);   /* ZD-1: the ONE execution-order walk runs ONCE per chain, before any emission, over the same nodes[] the loop below lays down */
    { g_emit.flat_all_zd = 0; if (!g_emit.flat_jmp_entry && !g_emit.flat_pat && !g_emit.flat_gen && x86_zc_frame() == ZC_FRAME_RSP && n > 0) { int _azd = 1; for (int _i = 0; _i < n; _i++) if (!zd_on[_i]) { _azd = 0; break; } g_emit.flat_all_zd = _azd; } { static int _lpd = -1; if (_lpd < 0) { const char * _e = getenv("SCRIP_LP_DIAG"); _lpd = (_e && *_e == '1') ? 1 : 0; } if (_lpd) { int _arm = 0; for (int _i = 0; _i < n; _i++) if (zd_on[_i]) _arm++; fprintf(stderr, "[LP] prefix=%s n=%d armed=%d all_zd=%d region=%d jmp=%d pat=%d gen=%d\n", prefix, n, _arm, g_emit.flat_all_zd, (g_emit_cfg ? g_emit_cfg->jcon_value_region : -1), g_emit.flat_jmp_entry, g_emit.flat_pat, g_emit.flat_gen); } } }   /* LP-2 LAYOUT PASS (s22d NEXT(1)): the verdict is now the ZD PLAN ITSELF, not a kind approximation.  The RPO walk + zd_plan moved ABOVE the prologue (both are PURE -- verified: zero emissions, zero g_emit writes in either), so `flat_all_zd` reads the REAL per-node arm results zd_on[] that the drive loop will use.  ONE AUTHORITY: the predicate and the emission now cannot disagree, because they are the same array.  LP-1 asked "do these kinds LOOK armable?" and could answer yes where zd_plan then declined a statement for a STRUCTURAL reason (cross-statement operand, staging failure) -- leaving legacy FR/FRQ readers live against a carve LP-1 had already zeroed.  This asks "did every node ACTUALLY arm?", so flat_all_zd=1 now means the carve is zero BY CONSTRUCTION.  zd_plan self-declines jmp-entry/non-FORTH (leaving zon all-zero, hence all_zd=0); the flat_pat/flat_gen/RSP guards are kept explicit so the flag means what it says at its source.  Widens automatically as kinds arm -- when IR_CALL lands (ZD-7) call-bearing graphs become carve-free with no edit here, which is the DL-ONESHOT rbp-free classifier seed/test_sno_dl_2_norbp.s asks for. */
    { extern void emit_fb_divergence_check(void); emit_fb_divergence_check(); }   /* ZETA-FB-1 (s160): the LAST point at which every input to BOTH frame-base predicates is final for this graph and the prologue has not yet run — x86_fb_pinned()=emit_jmp_pin_rbp() picks the base every DATA ref NAMES, emit_rec_pin() picks the base the RECORD protocol names, and the prologue dispatched on the next line is what ESTABLISHES one.  A disagreement here is the s158 land mine latent: store and load naming different registers.  Reports under SCRIP_FB_DIVERGE=1, costs one compare otherwise. */
    if (text_externalise && g_is_text) emit_label_define_bb(&lbl_α);
    xa_dispatch(XA_FLAT_PROLOGUE);
    if (g_is_text) g_emit_pos += 7;
    bb_label_t lbl_attempt, lbl_scanhit, lbl_scanfail;
    bb_label_t lbl_stcγ, lbl_stcω;   /* STMT-FRAME (s21x-c): the two chain-exit LEAVE cuts -- every statement-crossing edge into lbl_γ/lbl_ω funnels through these (mov rsp,rbp; pop rbp; jmp), the seed's stmt_MAIN_1_end/_fail shape at chain scope; bodies emitted just before the lbl_γ define */
    if (g_emit.flat_stmt_frame) { emit_label_initf(&lbl_stcγ, "%s_stγ", prefix); emit_label_initf(&lbl_stcω, "%s_stω", prefix); }
    static int _scan_off = -1; if (_scan_off < 0) { const char *_e = getenv("SCRIP_SCAN_OFF"); _scan_off = (_e && *_e == '1') ? 1 : 0; }   /* SPD-2 hatch: SCRIP_SCAN_OFF=1 = same-build A/B (BP-5/BP-6 precedent) */
    int scan_live = (!_scan_off && x86_zc_frame() == ZC_FRAME_RSP && g_emit.flat_pat) ? 1 : 0;   /* SPD-2 RETRY-INTERNAL: RSP/rbp flavor only (fb==rbp hardwired below); legacy frame modes keep the classic per-position round trip */
    if (scan_live) {
        emit_label_initf(&lbl_attempt,  "%s_attempt",  prefix);
        emit_label_initf(&lbl_scanhit,  "%s_scanhit",  prefix);
        emit_label_initf(&lbl_scanfail, "%s_scanfail", prefix);
        int kt = g_emit.flat_frame_bytes;
        int _sd1 = kt - 32, _sd2 = kt - 40;
        if (g_is_text) {
            char _sc[192];
            snprintf(_sc, sizeof _sc, "mov qword ptr [rsp + %d], r8\nmov dword ptr [rsp + %d], r14d\n", _sd1, _sd2);   /* SCANBASE: rsp-based — these two stores run before any rsp motion past the jmp-entry seed (only [rsp+N] zero-fills intervene), so rsp==rbp==base by construction; [kt-32]=scan flag (caller r8: defer fast arm is the SOLE flat_pat entry), [kt-40]=attempt start */
            emit_text_n(_sc, strlen(_sc));
        } else {
            ef_b2(0x4C, 0x89); if (_sd1 >= -128 && _sd1 <= 127) { ef_b3(0x44, 0x24, (uint8_t)(int8_t)_sd1); } else { ef_b2(0x84, 0x24); bb_emit_u32((uint32_t)_sd1); }
            ef_b2(0x44, 0x89); if (_sd2 >= -128 && _sd2 <= 127) { ef_b3(0x74, 0x24, (uint8_t)(int8_t)_sd2); } else { ef_b2(0xB4, 0x24); bb_emit_u32((uint32_t)_sd2); }
        }
        emit_label_define_bb(&lbl_attempt);
    }
    emit_label_define_bb(&lbl_α_body);
    (void)scan_live;
    bb_label_t **lbls  = (bb_label_t **)alloca(sizeof(bb_label_t *) * n);
    bb_label_t **betas = (bb_label_t **)alloca(sizeof(bb_label_t *) * n);
    bb_label_t **ra_y  = (bb_label_t **)alloca(sizeof(bb_label_t *) * n);
    bb_label_t **ra_t  = (bb_label_t **)alloca(sizeof(bb_label_t *) * n);
    bb_label_t **na_s  = (bb_label_t **)alloca(sizeof(bb_label_t *) * n);
    bb_label_t ***fc_sig = (bb_label_t ***)alloca(sizeof(bb_label_t **) * n);
    bb_label_t **na_f  = (bb_label_t **)alloca(sizeof(bb_label_t *) * n);
    bb_label_t **st_pre = (bb_label_t **)alloca(sizeof(bb_label_t *) * n);   /* STMT-FRAME (s21x-c): per-statement-head enter stub label -- minted for every bb_src_of head when the regime is live, NULL otherwise; every cross-edge into a head lbls[k] retargets here (slice-1 whitelist has no gotos, so EVERY edge into a head is a genuine statement entry -- no membership map needed) */
    int st_first_seen = 0;
    bb_label_t **st_x   = (bb_label_t **)alloca(sizeof(bb_label_t *) * n);   /* STMT-FRAME: the head's LEAVE half -- a statement-crossing edge must cut the SOURCE bracket before the dest's enter (the seed's stmt_*_end/_fail law: leave THEN jmp), so st_x[k] emits bb_glue_framed_leave (GLUE-4) and FALLS THROUGH into st_pre[k].  NULL for the first-emitted head: its only entrant is the prologue fallthrough, which has no bracket to cut (and cutting there would load the CRT caller's rbp into rsp -- the 058 disease). */
    for (int i = 0; i < n && g_flat_chain_set_n < FLAT_CHAIN_SET_MAX; i++) g_flat_chain_set[g_flat_chain_set_n++] = nodes[i];
    g_suspend_resume_slot = -1;
    if (g_gen_proc_active && g_emit_cfg && g_emit_cfg->resume_slot >= 0)
        for (int _si = 0; _si < n; _si++) if (nodes[_si]->op == IR_SUSPEND) { g_suspend_resume_slot = g_emit_cfg->resume_slot; break; }
    if (g_suspend_resume_slot < 0 && g_resumable_callable_active && g_emit_cfg && g_emit_cfg->resume_slot >= 0)
        g_suspend_resume_slot = g_emit_cfg->resume_slot;
    static int _diag_chain_seq = 0; int id = _diag_chain_seq++;   /* SN4-ASM-CRIT: diag-only ordinal for the SEQVERDICT / DRIVE-DIAG stderr prints — no longer names labels and no longer draws from the g_flat_node_id uid stream */
    /* RUNG ZB-OWN-0 (Lon 2026-07-11): per-chain statement mark = HEAD's zls entry +16 (head.zls2_mark quad);
     * -1 when the chain has no HEAD (non-match chains carry no per-box shadow at rung 0 -- recorded scope cut). */
    int own_mark = -1;
    { extern int zls_off(const IR_t *); for (int _oi = 0; _oi < n; _oi++) if (nodes[_oi]->op == IR_MATCH_HEAD) { int _ho = zls_off(nodes[_oi]); if (_ho >= 0) own_mark = _ho + 16; break; } }
    for (int i = 0; i < n; i++) {
        int _uid = g_flat_node_id++; const char * _kn = flat_label_kind(nodes[i]->op);
        lbls[i]  = emit_label_alloc("n%d_%s_α", _uid, _kn);
        betas[i] = emit_label_alloc("n%d_%s_β", _uid, _kn);
        ra_y[i]  = (nodes[i]->op == IR_REPALT) ? emit_label_alloc("n%d_%s_ry", _uid, _kn) : NULL;
        ra_t[i]  = (nodes[i]->op == IR_REPALT) ? emit_label_alloc("n%d_%s_rt", _uid, _kn) : NULL;
        na_s[i]  = ((nodes[i]->op == IR_MATCH_ALTERNATE || nodes[i]->op == IR_MATCH_SEQUENCE || nodes[i]->op == IR_MATCH_ARBNO || nodes[i]->op == IR_MATCH_FENCE1 || nodes[i]->op == IR_SCAN_SEQUENCE || nodes[i]->op == IR_SCAN_ALTERNATE || nodes[i]->op == IR_DISJUNCTION) && nodes[i]->n_operands > 0) ? emit_label_alloc("n%d_%s_as", _uid, _kn) : NULL;   /* SN4-NARY-ALT/-SEQ success-glue */
        na_f[i]  = ((nodes[i]->op == IR_MATCH_ALTERNATE || nodes[i]->op == IR_MATCH_SEQUENCE || nodes[i]->op == IR_MATCH_ARBNO || nodes[i]->op == IR_MATCH_FENCE1 || nodes[i]->op == IR_SCAN_SEQUENCE || nodes[i]->op == IR_SCAN_ALTERNATE || nodes[i]->op == IR_DISJUNCTION) && nodes[i]->n_operands > 0) ? emit_label_alloc("n%d_%s_af", _uid, _kn) : NULL;   /* SN4-NARY-ALT/-SEQ fail-glue */
        { extern const char * bb_src_of(const IR_t *); st_pre[i] = (g_emit.flat_stmt_frame && bb_src_of(nodes[i])) ? emit_label_alloc("n%d_%s_st", _uid, _kn) : NULL; }   /* STMT-FRAME: head stub label at every source-statement head node */
        st_x[i] = (st_pre[i] && st_first_seen) ? emit_label_alloc("n%d_%s_sx", _uid, _kn) : NULL; if (st_pre[i]) st_first_seen = 1;   /* STMT-FRAME: leave half, skipped for the first-emitted head (prologue entrant only) */
        fc_sig[i] = NULL;   /* ZB-FC-3a: per-arm sigma pad-stub labels for a LINEAR-ARM granted ALT (FORTH flavor) */
        if (nodes[i]->op == IR_MATCH_ALTERNATE && nodes[i]->n_operands > 0) {   /* ALT-FLAT s202: sigma stubs for EVERY match-alt (they now store the resume continuation, universal address dispatch), not just FORTH-granted ones */
            int _N = (int)(nodes[i]->n_operands / 2);
            fc_sig[i] = (bb_label_t **)alloca(sizeof(bb_label_t *) * _N);
            for (int _j = 0; _j < _N; _j++) fc_sig[i][_j] = emit_label_alloc("n%d_%s_s%d", _uid, _kn, _j);
        }
    }
    unsigned char *bused = (unsigned char *)alloca(n > 0 ? n : 1);
    flat_beta_used_scan(nodes, n, bused);
    unsigned char *seqclean = (unsigned char *)alloca(n > 0 ? n : 1);
    /* SPD SEQ-STATIC prepass: a celled IR_MATCH_SEQUENCE converts to the zero-counter statically-wired arm iff (a) every element entry AND resume root is present in this chain (sigma_i -> alpha_{i+1} / phi_i -> beta_{i-1} all resolvable) and (b) every sigma/phi-marked edge into it originates FROM one of those roots (GOTO-chased marks from foreign protocol glue -- DEFER return, ARBNO seal -- must keep the counter dispatch: degrade never die).  FC-converted SEQs bypass this (eligibility already fences their source set). */
    static int _sqoff = -1; if (_sqoff < 0) { const char *_e = getenv("SCRIP_SEQSTATIC"); _sqoff = (_e && *_e == '0') ? 1 : 0; }   /* SPD SEQ-STATIC: DEFAULT ON since s110 -- the s109 "non-sigma/phi glue consumer" was the ZPOP-FOLD chase below (phi hop into a converted SEQ kept the raw af glue, jmp-omega under conversion; bracketed via SCRIP_ZPOP_FOLD_OFF=1 discrimination, fixed by mirroring the main loop's fc_seq_phi_tgt redirect at the fold's hop resolution).  SCRIP_SEQSTATIC=0 is the same-build A/B hatch (BP-5/BP-6 precedent). */
    for (int k = 0; k < n; k++) {
        seqclean[k] = 0;
        if (_sqoff || nodes[k]->op != IR_MATCH_SEQUENCE || nodes[k]->n_operands < 2) continue;
        int N = (int)(nodes[k]->n_operands / 2), ok = 1;
        for (int j = 0; j < N && ok; j++) { int fe = 0, fr = 0; for (int p = 0; p < n; p++) { if (nodes[p] == nodes[k]->operands[2 * j]) fe = 1; if (nodes[p] == nodes[k]->operands[2 * j + 1]) fr = 1; } if (!fe || !fr) ok = 0; }
        seqclean[k] = (unsigned char)ok;
    }
    { static int _sqmax = -2; if (_sqmax < -1) { const char *_e = getenv("SCRIP_SEQSTATIC_MAX"); _sqmax = _e ? atoi(_e) : -1; }
      static int _sqcnt = 0;
      if (_sqmax >= 0) for (int k = 0; k < n; k++) if (seqclean[k] && nodes[k]->op == IR_MATCH_SEQUENCE) { if (_sqcnt >= _sqmax) seqclean[k] = 0; else { static int _sd2 = -1; if (_sd2 < 0) { const char *_e2 = getenv("SCRIP_BLOB_MAP"); _sd2 = (_e2 && *_e2 == '1') ? 1 : 0; } if (_sd2) fprintf(stderr, "SEQCONV idx=%d k=%d N=%d\n", _sqcnt, k, (int)(nodes[k]->n_operands / 2)); _sqcnt++; } } }
    { int *claim = (int *)alloca(sizeof(int) * (n > 0 ? n : 1));
      /* SPD SEQ-STATIC DAG fence: SNOBOL4 patterns are VALUES -- a reused pattern variable lowers as a shared SUBTREE, so one physical root can be an element of TWO sequences.  Its single sigma/phi edge can be statically aimed at only ONE parent's neighbor; the counter glue was the DAG's runtime disambiguator.  Any root claimed by >= 2 sequences dirties ALL claimants (degrade never die). */
      for (int p = 0; p < n; p++) claim[p] = -1;
      for (int k = 0; k < n; k++) if (seqclean[k]) { int N = (int)(nodes[k]->n_operands / 2);
        for (int j = 0; j < 2 * N; j++) for (int p = 0; p < n; p++) if (nodes[p] == nodes[k]->operands[j]) { if (claim[p] >= 0 && claim[p] != k) { seqclean[k] = 0; seqclean[claim[p]] = 0; } else claim[p] = k; break; } } }
    for (int i = 0; i < n; i++) for (int side = 0; side < 2; side++) {
        IR_t *t = side ? nodes[i]->ω.node : nodes[i]->γ.node;
        const char *z = side ? nodes[i]->ω.sz : nodes[i]->γ.sz;
        int mk = (z[0] == (char)0xcf && ((unsigned char)z[1] == 0x83 || (unsigned char)z[1] == 0x86));
        { int _gg = 0; IR_t *_c = t; while (_c && _c->op == IR_GOTO && _gg++ < 128) { if (!mk) mk = (_c->γ.sz[0] == (char)0xcf && ((unsigned char)_c->γ.sz[1] == 0x83 || (unsigned char)_c->γ.sz[1] == 0x86)); t = _c->γ.node; _c = t; } }
        if (!mk || !t || t->op != IR_MATCH_SEQUENCE) continue;
        for (int k = 0; k < n; k++) if (nodes[k] == t && seqclean[k]) {
            int N = (int)(nodes[k]->n_operands / 2), root = 0;
            for (int j = 0; j < N; j++) if (nodes[k]->operands[2 * j] == nodes[i] || nodes[k]->operands[2 * j + 1] == nodes[i]) root = 1;
            if (!root) seqclean[k] = 0;
        }
    }
    { static int _sd = -1; if (_sd < 0) { const char *_e = getenv("SCRIP_BLOB_MAP"); _sd = (_e && *_e == '1') ? 1 : 0; }
      if (_sd) for (int k = 0; k < n; k++) if (nodes[k]->op == IR_MATCH_SEQUENCE) { fprintf(stderr, "SEQVERDICT chain=%d k=%d N=%d clean=%d fc=%d elems=", id, k, (int)(nodes[k]->n_operands / 2), (int)seqclean[k], fc_seq_on(nodes[k]) ? 1 : 0); for (int j = 0; j < nodes[k]->n_operands; j++) { int _f = -1; for (int p = 0; p < n; p++) if (nodes[p] == nodes[k]->operands[j]) { _f = p; break; } fprintf(stderr, "%d,", _f); } fprintf(stderr, "\n"); } }
    bb_label_t *resume_init_lbl = NULL;
    if (g_suspend_resume_slot >= 0 && g_gen_proc_active) {
        for (int _si = 0; _si < n; _si++) if (nodes[_si]->op == IR_SUSPEND) { resume_init_lbl = betas[_si]; break; }
    } else if (g_suspend_resume_slot >= 0 && g_resumable_callable_active) {
        /* NCB-2/SZ-1 (FINDING-2026-07-10 blob-β-resume): a resumable-callable chain's α initializes the
         * continuation slot to LOWER-recorded body_root's β — the pattern subgraph's FIRST-ALLOCATED node,
         * i.e. the rightmost leaf under right-first lowering, the exact sno_resume_ω_to / v2 F.γ convention
         * ("the exhaust box resumes the body via F.γ → the body's FIRST-ALLOCATED node's β").  An esi=1
         * re-entry then behaves as "the element right of the whole pattern failed": the normal right-to-left
         * β cascade extends interior generators (ARBNO) or ripples to exhaustion → lbl_ω → fail-return.
         * SZ-2c's dynamic MARK store rewrites this same slot with the same lea/mov idiom — one mechanism. */
        if (g_emit_cfg && g_emit_cfg->body_root)
            for (int _ti = 0; _ti < n; _ti++) if (nodes[_ti] == g_emit_cfg->body_root) { resume_init_lbl = betas[_ti]; break; }
        if (!resume_init_lbl) resume_init_lbl = &lbl_ω;
    }
    if (resume_init_lbl) {
        if (g_is_text) {
            char _init[256];
            snprintf(_init, sizeof _init,
                "lea rax, [rip + %s]\nmov qword ptr [%s + %d], rax\n",
                resume_init_lbl->name, emit_rec_fb(), g_suspend_resume_slot);   /* REG-7 U5: the resume slot is a FRAME slot — fb, not zr (zr sealed to rsp; the old flat_pat zr="rbp" coincidence is gone) */
            emit_text_n(_init, strlen(_init));
        } else {
            ef_b3(0x48, 0x8D, 0x05); bb_emit_patch_rel32(resume_init_lbl);
            { int z = emit_rec_fb_num(), lo = z & 7; ef_b2((uint8_t)(0x48 | (z >= 8 ? 0x01 : 0x00)), 0x89); if (lo == 4) ef_b2(0x84, 0x24); else ef_b1((uint8_t)(0x80 | lo)); bb_emit_u32((uint32_t)(unsigned)g_suspend_resume_slot); }   /* REG-7 U5: fb twin */
        }
    }
    for (int _li = 0; _li < n; _li++) if (nodes[_li]->op == IR_LIMIT) {
        g_emit.op_off = drive_value_slot(nodes[_li]);
        bb_emit_x86(bb_limit_init());
    }
    for (int i = 0; i < n; i++) {
        if (g_emit.flat_stmt_frame && st_pre[i]) { g_emit.xa_bb_emit_pair_n = 2; g_emit.xa_bb_emit_pair_define[0] = st_x[i]; g_emit.xa_bb_emit_pair_jmp[0] = NULL; g_emit.xa_bb_emit_pair_define[1] = st_pre[i]; g_emit.xa_bb_emit_pair_jmp[1] = NULL; g_emit.op_fc_bytes = 0; bb_emit_x86((st_x[i] ? x86_deflabel_pair(0) + bb_glue_framed_leave() : std::string()) + x86_deflabel_pair(1) + bb_glue_framed_enter()); }   /* STMT-FRAME head stub: [st_x: cut the SOURCE bracket (mov rsp,rbp; pop rbp) --] st_pre: rbp dance forward, fall through into the node's own α.  Cross-statement edges land st_x (leave-then-enter, the seed's stmt_*_end/_fail law); the prologue and nothing else falls into the first head's bare st_pre.  GLUE-4 (s21x-p): the bracket IS bb_glue_framed_* at K=0 -- op_fc_bytes zeroed HERE (driver-handoff, R5b) because this stub fires between statements where `_` still holds the previous node's K; the glue's pad arithmetic then reproduces the old x86_stmt_enter bytes exactly, and STATEMENT becomes the framed glue's first live customer (s21x-c law 4).  Both media via the pair 'E' records + the s21x-c encoders. */
        /* BB-OWNED-ζ / ONE-WAY-OUTPUT FIX (Claude, this session, per Lon's ask): α's byte position used to be
         * defined HERE, directly, via a bare bb_label_define(lbls[i]) call outside x86()'s funnel entirely --
         * a second output path for the identical logical operation x86_beta() already performs correctly
         * through x86_deflabel()/x86_Drec(), one function, both media, internally.  That direct call is now
         * REMOVED; every template defines its OWN α via x86_alpha() as its first emitted instruction
         * (mirroring the existing x86_beta() convention), so bb_label_define(lbls[i]) fires exactly once,
         * from inside walk_bb_node's own bb_emit_x86 walk of that template's returned 'D'-tagged record --
         * the SAME single call site β already uses.  Scope: this fixes ONLY the per-node α/β def-mechanism
         * asymmetry; it does not audit or touch any other TEXT/BINARY-branching site in this file. */
        emit_zeta_selfload();
        { static int _beo = -1; if (_beo < 0) { const char *_e = getenv("SCRIP_BETA_ELIDE_OFF"); _beo = (_e && _e[0] == '1') ? 1 : 0; } g_emit.op_beta_dead = (_beo || bused[i]) ? 0 : 1; }   /* BP-9 DEAD-β: hatch = same-build A/B, BP-5/BP-6 precedent */
        g_emit.op_wpop = 0;   /* BP-9 (ii): zeroed at TOP of every iteration -- the REPALT/ALT-family dispatches continue before the chase block below, and a stale ΣK from a prior node would poison their emitted jmp-ω hooks (the recorded op_sb/lbl_t1 g_emit-persistence hazard class) */
        { extern int zls_off(const IR_t *); extern int zls_result_off(const IR_t *); extern int zls_node_bytes(const IR_t *); int _zo = zls_off(nodes[i]); int _zr = zls_result_off(nodes[i]);
          g_emit.op_own_mark = own_mark; g_emit.op_own_ci = (_zo >= 0) ? ((_zr >= 0 ? _zr : _zo) + zls_node_bytes(nodes[i])) : 0; }   /* C_i BASE FIX: zls_off returns e->loff (the SHIFTED locals base for the zls_locals_shifted family) while zls_node_bytes measures from e->off (the RESULT base), so loff+bytes overstated every shifted node's end by exactly the shift -- e.g. IR_MATCH_BREAK result@63792 + cnt/cur@63808 (true end 63824) reported ci=63840, i.e. 16 bytes INTO the next node. Same base-convention class as ZLS-CALL-BASE. C_i means "everything left of the live box allocated" (ZC_PORT_OWNED, zeta_choices.h), so the node END is result_off+bytes. Non-shifted nodes have loff==off and are unaffected. */
        bb_label_t *node_γ = &lbl_γ;
        bb_label_t *node_ω = &lbl_ω;
        IR_t *gtgt = nodes[i]->γ.node;
        IR_t *otgt = nodes[i]->ω.node;
        int gamma_is_beta = (nodes[i]->γ.sz[0] == (char)0xce && (unsigned char)nodes[i]->γ.sz[1] == 0xb2);
        int omega_is_beta = (nodes[i]->ω.sz[0] == (char)0xce && (unsigned char)nodes[i]->ω.sz[1] == 0xb2);
        int gamma_is_sig  = (nodes[i]->γ.sz[0] == (char)0xcf && (unsigned char)nodes[i]->γ.sz[1] == 0x83);   /* SN4-NARY-ALT "σ": inside-success → target's na_s glue */
        int gamma_is_phi  = (nodes[i]->γ.sz[0] == (char)0xcf && (unsigned char)nodes[i]->γ.sz[1] == 0x86);   /* SN4-NARY-ALT "φ": inside-fail → target's na_f glue */
        int omega_is_phi  = (nodes[i]->ω.sz[0] == (char)0xcf && (unsigned char)nodes[i]->ω.sz[1] == 0x86);
        { int _gg = 0; IR_t * _g = gtgt; while (_g && _g->op == IR_GOTO && _gg++ < 128) { if (!gamma_is_beta) gamma_is_beta = (_g->γ.sz[0] == (char)0xce && (unsigned char)_g->γ.sz[1] == 0xb2); if (!gamma_is_sig) gamma_is_sig = (_g->γ.sz[0] == (char)0xcf && (unsigned char)_g->γ.sz[1] == 0x83); if (!gamma_is_phi) gamma_is_phi = (_g->γ.sz[0] == (char)0xcf && (unsigned char)_g->γ.sz[1] == 0x86); gtgt = _g->γ.node; _g = gtgt; } }
        { int _gg = 0; IR_t * _o = otgt; while (_o && _o->op == IR_GOTO && _gg++ < 128) { if (!omega_is_beta) omega_is_beta = (_o->γ.sz[0] == (char)0xce && (unsigned char)_o->γ.sz[1] == 0xb2); if (!omega_is_phi) omega_is_phi = (_o->γ.sz[0] == (char)0xcf && (unsigned char)_o->γ.sz[1] == 0x86); otgt = _o->γ.node; _o = otgt; } }
        for (int k = 0; k < n; k++) if (nodes[k] == gtgt) {
            node_γ = (gamma_is_phi && na_f[k]) ? na_f[k] : (gamma_is_sig && na_s[k]) ? na_s[k] : gamma_is_beta ? betas[k] : lbls[k];
            /* ZB-FC-3a: a sigma edge into a LINEAR-ARM granted ALT lands the SOURCE ARM's pad stub instead of
             * na_s directly -- the stub subs (FPMAX - fp_arm) so every arm yields at the uniform padded depth
             * (S10d).  arm_of(source) = largest j with pos(entry_j) <= pos(source) in nodes[] order (arms are
             * allocated contiguously after A; entry_j is each arm's first-allocated node, the s31 convention). */
            if (gamma_is_sig && fc_sig[k]) {   /* ALT-FLAT s202: stubs exist for every match-alt; label presence is the gate */
                /* arm_of(source) = the arm whose entry is NEAREST AT-OR-BELOW pos(source).  The prior "largest j with pos(entry_j) <= pos(source)" assumed arm entries allocate ASCENDING, but
                 * right-first lowering allocates them DESCENDING (arm N-1's nodes get the LOWEST indices), so every source at-or-after entry_0 spuriously matched the last passing j — measured s71:
                 * SPAN('a')|'b' routed SPAN's yield (already at full arm depth) through the literal arm's +16 pad stub, desyncing every later [rsp+const] until rsp died.  Equal-fp arms (163's
                 * literal|literal, all stubs identical) masked it.  Nearest-below is order-agnostic: each arm's nodes sit contiguously at-or-after its own entry. */
                int _N = (int)(nodes[k]->n_operands / 2), _arm = 0, _src = -1, _best = -1;
                for (int _p = 0; _p < n; _p++) if (nodes[_p] == nodes[i]) { _src = _p; break; }
                for (int _j = 0; _j < _N; _j++) { IR_t *_e = nodes[k]->operands[2 * _j]; for (int _p = 0; _p < n; _p++) if (nodes[_p] == _e) { if (_p <= _src && _p > _best) { _best = _p; _arm = _j; } break; } }
                node_γ = fc_sig[k][_arm];
            }
            if (gamma_is_sig && (fc_seq_on(nodes[k]) || (seq_static_on(nodes[k]) && seqclean[k]))) { node_γ = fc_seq_sigma_tgt(nodes, n, k, i, lbls,  node_γ); { static int _sd = -1; if (_sd < 0) { const char *_e = getenv("SCRIP_BLOB_MAP"); _sd = (_e && *_e == '1') ? 1 : 0; } if (_sd) fprintf(stderr, "SEQRP sig chain=%d src=%d k=%d el=%d tgt=%s\n", id, i, k, fc_seq_elem_of(nodes, n, k, i), node_γ ? node_γ->name : "?"); } }   /* ZB-FC-3b: inside-success → NEXT element's α (static; rsp already correct) */
            if (gamma_is_phi && (fc_seq_on(nodes[k]) || (seq_static_on(nodes[k]) && seqclean[k]))) { node_γ = fc_seq_phi_tgt  (nodes, n, k, i, betas, node_γ); { static int _sd = -1; if (_sd < 0) { const char *_e = getenv("SCRIP_BLOB_MAP"); _sd = (_e && *_e == '1') ? 1 : 0; } if (_sd) fprintf(stderr, "SEQRP gphi chain=%d src=%d k=%d el=%d tgt=%s\n", id, i, k, fc_seq_elem_of(nodes, n, k, i), node_γ ? node_γ->name : "?"); } }   /* ZB-FC-3b: a FAIL-goto's γ means fail → PREV element's β */
            break;
        }
        if (nodes[i]->γ.node == NULL || nodes[i]->γ.node->op == IR_SUCCEED) node_γ = &lbl_γ;
        if (nodes[i]->γ.node && nodes[i]->γ.node->op == IR_FAIL) node_γ = &lbl_ω;
        int omega_resolved = 0;
        for (int k = 0; k < n; k++) if (nodes[k] == otgt) { node_ω = (omega_is_phi && na_f[k]) ? na_f[k] : omega_is_beta ? betas[k] : lbls[k]; if (omega_is_phi && (fc_seq_on(nodes[k]) || (seq_static_on(nodes[k]) && seqclean[k]))) { node_ω = fc_seq_phi_tgt(nodes, n, k, i, betas, node_ω); { static int _sd = -1; if (_sd < 0) { const char *_e = getenv("SCRIP_BLOB_MAP"); _sd = (_e && *_e == '1') ? 1 : 0; } if (_sd) fprintf(stderr, "SEQRP ophi chain=%d src=%d k=%d el=%d tgt=%s\n", id, i, k, fc_seq_elem_of(nodes, n, k, i), node_ω ? node_ω->name : "?"); } } omega_resolved = 1; break; }   /* ZB-FC-3b: inside-fail → PREV element's β (static; subtree already popped) */
        if (!omega_resolved) node_ω = (otgt && otgt->op == IR_SUCCEED) ? &lbl_γ : &lbl_ω;
        /* BB-OWNED-ζ STEP 1: true death iff this ω-edge falls all the way through to the chain's own outer
         * lbl_ω (never resolved inside the local chain) AND the unresolved target isn't a success-fallthrough
         * masquerading through the ω port.  See the field comment in emit.h for the full reasoning; verified
         * against IR_MATCH_ARBNO roles 2/5 via lower_snobol4.c's sno_ω_to(F, fail) construction this session. */
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
            else for (int k = 0; k < n; k++) if (nodes[k] == rtgt) { g_move_label_tgt = wantb ? betas[k] : lbls[k]; break; }
        }
        if (nodes[i]->op == IR_MOVE_LABEL && nodes[i]->n_operands > 1 && nodes[i]->operands[1]) {
            IR_t *igp = nodes[i]->operands[1];
            IR_t *sg = igp->γ.node;
            int sg_is_beta = (igp->γ.sz[0] == (char)0xce && (unsigned char)igp->γ.sz[1] == 0xb2);
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
        if ((nodes[i]->op == IR_MATCH_ALTERNATE || nodes[i]->op == IR_MATCH_SEQUENCE || nodes[i]->op == IR_MATCH_ARBNO || nodes[i]->op == IR_MATCH_FENCE1 || nodes[i]->op == IR_SCAN_SEQUENCE || nodes[i]->op == IR_SCAN_ALTERNATE || nodes[i]->op == IR_DISJUNCTION) && nodes[i]->n_operands > 0) {
            { static int _dd = -1; if (_dd < 0) { const char *_e = getenv("SCRIP_DRIVE_DIAG"); _dd = (_e && _e[0] == '1') ? 1 : 0; }
              long _p0 = _dd ? emit_text_count() : 0;
              { g_seq_static_cur = (nodes[i]->op == IR_MATCH_SEQUENCE) ? (int)seqclean[i] : 0; flat_drive_match_alt(nodes, n, i, lbls, betas, na_s, na_f, fc_sig, node_γ, node_ω, &lbl_ω); g_seq_static_cur = 0; }   /* SN4-NARY-SEQ rides the identical (entry,resume)×N + 2-glue pair layout; SPD SEQ-STATIC verdict rides the file-static into the drive dispatch */
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
        /* BP-9 (ii) ΣK ζ-POP FOLD (the accumulate mechanism proper; LIVE CURSOR residual (a)): when this node's
         * ω wire resolved to a WHITELISTED trivial-β trampoline (flat_trivial_beta -- template β is exactly
         * x86_beta_trampoline, so its emitted body is [hook add rsp,K]* jmp <its-own-resolved-ω>), inline that
         * body at the site: accumulate the fc-cell K, re-resolve the target's OWN ω by the loop's exact rules
         * (β/φ tag sniff through IR_GOTO, na_f glue, fc_seq_phi_tgt, REPALT override), iterate while the next
         * hop is again a whitelisted β.  node_ω lands the chain's FINAL label; op_wpop carries ΣK; the
         * X86H_JMP/OMEGA hook arm spends it (conditional ω rides the x86_jcc invert synth).  FORTH+RSP only --
         * the pops are rsp-discipline, and under any other port flavor the trampoline bodies differ.  Hatch:
         * SCRIP_ZPOP_FOLD_OFF=1 = same-build byte-identical baseline (SCRIP_BETA_ELIDE_OFF precedent). */
        { static int _zpf = -1; if (_zpf < 0) { const char *_e = getenv("SCRIP_ZPOP_FOLD_OFF"); _zpf = (_e && _e[0] == '1') ? 1 : 0; }
          if (!_zpf && x86_zc_frame() == ZC_FRAME_RSP && x86_port_mode() == ZC_PORT_FORTH) {
              long _sum = 0; int _fk = -1, _hops = 0;
              for (int _k = 0; _k < n; _k++) if (node_ω == betas[_k]) { _fk = _k; break; }
              while (_fk >= 0 && _hops++ < 64 && flat_trivial_beta(nodes[_fk])) {
                  { long _fck = 0; if (fc_geom(nodes[_fk], &_fck)) _sum += _fck; }
                  IR_t *_c = nodes[_fk]; IR_t *_o = _c->ω.node;
                  int _ib = (_c->ω.sz[0] == (char)0xce && (unsigned char)_c->ω.sz[1] == 0xb2);
                  int _ip = (_c->ω.sz[0] == (char)0xcf && (unsigned char)_c->ω.sz[1] == 0x86);
                  { int _gg = 0; IR_t *_g = _o; while (_g && _g->op == IR_GOTO && _gg++ < 128) { if (!_ib) _ib = (_g->γ.sz[0] == (char)0xce && (unsigned char)_g->γ.sz[1] == 0xb2); if (!_ip) _ip = (_g->γ.sz[0] == (char)0xcf && (unsigned char)_g->γ.sz[1] == 0x86); _o = _g->γ.node; _g = _o; } }
                  bb_label_t *_next = NULL; int _k2 = -1;
                  for (int _k = 0; _k < n; _k++) if (nodes[_k] == _o) { _k2 = _k; break; }
                  if (_k2 >= 0) { _next = (_ip && na_f[_k2]) ? na_f[_k2] : _ib ? betas[_k2] : lbls[_k2]; if (_ip && (fc_seq_on(nodes[_k2]) || (seq_static_on(nodes[_k2]) && seqclean[_k2]))) { bb_label_t *_b4 = _next; _next = fc_seq_phi_tgt(nodes, n, _k2, _fk, betas, _next); { static int _sd = -1; if (_sd < 0) { const char *_e = getenv("SCRIP_BLOB_MAP"); _sd = (_e && *_e == '1') ? 1 : 0; } if (_sd) fprintf(stderr, "SEQFOLD chain=%d fk=%d k2=%d el=%d raw=%s tgt=%s\n", id, _fk, _k2, fc_seq_elem_of(nodes, n, _k2, _fk), _b4 ? _b4->name : "?", _next ? _next->name : "?"); } } }   /* SPD SEQ-STATIC s110: the fold mirrors the main loop's rules -- a phi hop into a CONVERTED seq must take the same static re-point; the raw af glue is jmp-omega under conversion (the s109 treebank stmt-1 wire, bracketed via SCRIP_ZPOP_FOLD_OFF=1) */
                  else _next = (_o && _o->op == IR_SUCCEED) ? &lbl_γ : &lbl_ω;
                  for (int _r = 0; _r < n; _r++) if (nodes[_r]->op == IR_REPALT && nodes[_r]->n_operands > 0 && nodes[_r]->operands[0] == _c) { _next = ra_t[_r]; break; }
                  node_ω = _next;
                  _fk = -1; for (int _k = 0; _k < n; _k++) if (_next == betas[_k]) { _fk = _k; break; }   /* continue on ANY β landing -- fc_seq_phi_tgt redirects a φ edge to the PREV ELEMENT's β, a different node than k2 (measured: roman n9→n6 lands betas[5] via seq node 2) */
              }
              g_emit.op_wpop = (int)_sum;
          } }
        { static int _dd = -1; if (_dd < 0) { const char *_e = getenv("SCRIP_DRIVE_DIAG"); _dd = (_e && _e[0] == '1') ? 1 : 0; }
          long _p0 = _dd ? emit_text_count() : 0;
          g_emit.op_scan = 0; g_emit.op_scan_head_off = -1;
          if (scan_live) { if (node_γ == &lbl_γ) node_γ = &lbl_scanhit; if (node_ω == &lbl_ω) node_ω = &lbl_scanfail; }   /* SPD-2: every whole-pattern exit funnels through the scan blocks (ports bind node_γ/node_ω, so template port jumps are covered) */
          if (g_emit.flat_stmt_frame) {   /* STMT-FRAME retargets (SPD-2 idiom, one line above): edges into a statement head take its enter stub; edges out to the chain exits take the leave cuts.  Existing inline pops on those edges (the ΣK ω spends, the templates' own add rsp,N) become DEAD-BUT-HARMLESS: the cut's mov rsp,rbp normalizes rsp regardless of arrival depth -- the seed's "fail edges jump the bracket cut, zero hand-counted pops" law delivered without touching a single template. */
              for (int _k = 0; _k < n; _k++) { bb_label_t * _st = st_x[_k] ? st_x[_k] : st_pre[_k]; if (_st && node_γ == lbls[_k]) node_γ = _st; if (_st && node_ω == lbls[_k]) node_ω = _st; }
              if (node_γ == &lbl_γ) node_γ = &lbl_stcγ; if (node_γ == &lbl_ω) node_γ = &lbl_stcω;
              if (node_ω == &lbl_ω) node_ω = &lbl_stcω; if (node_ω == &lbl_γ) node_ω = &lbl_stcγ; }
          if (nodes[i]->op == IR_MATCH_DEFER || nodes[i]->op == IR_MATCH_PATREF) {   /* SPD-2 op_scan criterion: sole frozen-pattern statement root = head's γ (GOTO-chased) is THIS node AND this node's ω resolved to the head's β */
              int _hk = -1; for (int _k = 0; _k < n; _k++) if (nodes[_k]->op == IR_MATCH_HEAD) { _hk = _k; break; }
              if (_hk >= 0) { IR_t *_gt = nodes[_hk]->γ.node; { int _gg = 0; while (_gt && _gt->op == IR_GOTO && _gg++ < 128) _gt = _gt->γ.node; }
                  if (_gt == nodes[i] && node_ω == betas[_hk]) { g_emit.op_scan = 1; g_emit.op_scan_head_off = (int)drive_value_slot(nodes[_hk]); } }
          }
          if (zd_on[i] || zd_gp[i] > 0 || zd_wp[i] > 0) { g_zd_stage = 1; g_zd_arm = zd_on[i]; g_zd_gpop = zd_gp[i]; g_zd_wpop = zd_wp[i];
              g_zd_k = zd_on[i] ? zd_k(nodes[i]) : 0;   /* ZD-8 (b2) staged from zd_k -- THE ONE AUTHORITY, defined beside zd_nops.  This site and zd_plan's depth model were the SAME RULE SPELLED TWICE until s22k; both now call zd_k, so the planner's depth model and the emitted alpha carve cannot drift apart, and a change to the sink set is ONE edit rather than two-that-must-agree. */
              for (int _zj = 0; _zj < 6; _zj++) g_zd_read[_zj] = 0;
              if (zd_on[i]) { int _no = zd_nops(nodes[i]);
                  for (int _zj = 0; _zj < _no && _zj < 6; _zj++) { IR_t * _p = nodes[i]->operands[_zj]; for (int _k = 0; _k < n; _k++) if (nodes[_k] == _p) { g_zd_read[_zj] = zd_out[i] - zd_out[_k]; break; } } } }   /* ZD-1 staging (R5b driver-handoff): op_zread[k] = the DIFFERENCE OF TWO DEPTHS (s21x-u), zout[consumer] - zout[producer], both from the ONE walk.  ⛔ ZD-2d/2e FIX: this site RE-DERIVED the operand count inline (UNOP/ASSIGN?1:BINOP?2:0) instead of calling zd_nops, so the planner and the driver held TWO OPINIONS of how many operands a kind has -- the moment zd_nops grew COERCE_NUMERIC/CMP_TEST the planner ARMED their statements while this loop still computed _no=0, left g_zd_read all zero, and every ZOPQ/ZOPD rendered [rsp+0] = the box's OWN cell.  29 programs, every one silently wrong rather than crashing (rt_cmp_d comparing an uninitialized cell against itself).  ONE authority now; a kind added to zd_nops is staged here automatically.  K is no longer a second copy: s22k folded it into zd_k beside zd_nops, so BOTH one-authority laws now hold on this staging path. */
          emit_drive(nodes[i], lbls[i], node_γ, node_ω, betas[i]);
          if (_dd && emit_text_count() == _p0) fprintf(stderr, "[DRIVE-DIAG] ZERO-EMIT chain=%d i=%d op=%s n_operands=%d\n", id, i, bb_op_name(nodes[i]->op), nodes[i]->n_operands); }
    }
    g_emit.op_beta_dead = 0;
    g_emit.op_wpop = 0;
    if (scan_live) {   /* SPD-2 RETRY-INTERNAL blocks.  scanhit: publish the winning start for the statement-side head-slot write-back, fall to γ.  scanfail: flag off → γ.. no, → ω; else start+1, bound (r15d=Δ), &ANCHOR test — any exit → ω; otherwise write back start, r14d=start, rsp=rbp (post-carve frontier: every element grant sits below), jmp attempt. */
        int kt = g_emit.flat_frame_bytes;
        extern uint64_t g_scan_hit_start; extern long g_anchor;
        emit_label_define_bb(&lbl_scanhit);
        if (g_is_text) {
            char _sh[256];
            snprintf(_sh, sizeof _sh, "cmp qword ptr [rbp + %d], 1\njne 7f\nmov ecx, dword ptr [rbp + %d]\nlea rdx, [rip + g_scan_hit_start]\nmov dword ptr [rdx], ecx\n7:\n", kt - 32, kt - 40);
            emit_text_n(_sh, strlen(_sh));
        } else {
            ef_b3(0x48, 0x83, 0xBD); bb_emit_u32((uint32_t)(kt - 32)); ef_b1(0x01);
            ef_b2(0x75, 0x12);
            ef_b2(0x8B, 0x8D); bb_emit_u32((uint32_t)(kt - 40));
            ef_b2(0x48, 0xBA); bb_emit_u64((uint64_t)(uintptr_t)&g_scan_hit_start);
            ef_b2(0x89, 0x0A);
        }
        emit_jmp_label(&lbl_γ, JMP_JMP);
        emit_label_define_bb(&lbl_scanfail);
        if (g_is_text) {
            char _sf[512];
            snprintf(_sf, sizeof _sf,
                "cmp qword ptr [rbp + %d], 1\njne 8f\nmov eax, dword ptr [rbp + %d]\ninc eax\ncmp eax, r15d\njg 8f\nlea rcx, [rip + g_anchor]\ncmp qword ptr [rcx], 0\njne 8f\nmov dword ptr [rbp + %d], eax\nmov r14d, eax\nmov rsp, rbp\n",
                kt - 32, kt - 40, kt - 40);
            emit_text_n(_sf, strlen(_sf));
            emit_jmp_label(&lbl_attempt, JMP_JMP);
            emit_text_n("8:\n", 3);
        } else {
            ef_b3(0x48, 0x83, 0xBD); bb_emit_u32((uint32_t)(kt - 32)); ef_b1(0x01);
            ef_b2(0x75, 0x2E);                                    /* jne -> block end: 6+2+3+2+10+4+2+6+3+3+5 = 46 */
            ef_b2(0x8B, 0x85); bb_emit_u32((uint32_t)(kt - 40));
            ef_b2(0xFF, 0xC0);
            ef_b3(0x44, 0x39, 0xF8);
            ef_b2(0x7F, 0x21);                                    /* jg -> block end: 10+4+2+6+3+3+5 = 33 */
            ef_b2(0x48, 0xB9); bb_emit_u64((uint64_t)(uintptr_t)&g_anchor);
            ef_b4(0x48, 0x83, 0x39, 0x00);
            ef_b2(0x75, 0x11);                                    /* jne -> block end: 6+3+3+5 = 17 */
            ef_b2(0x89, 0x85); bb_emit_u32((uint32_t)(kt - 40));
            ef_b3(0x41, 0x89, 0xC6);
            ef_b3(0x48, 0x89, 0xEC);
            emit_jmp_label(&lbl_attempt, JMP_JMP);
        }
        emit_jmp_label(&lbl_ω, JMP_JMP);
    }
    if (g_emit.flat_jmp_entry) {
        /* PROC-CONV: EMPTY body (n==0) — α_body would otherwise fall into the res landing below and run its
         * `add rsp,8; pop zr` on the initial α pass (no frontier record present ⇒ rsp/zr corruption ⇒ the ω-half
         * then unwinds through a garbage frame reg and wild-jumps).  A bare `:(FRETURN)` terminates in IR_FAIL ⇒
         * jump to lbl_ω; a bare `:(RETURN)` / fall-off-end terminates in IR_SUCCEED ⇒ lbl_γ.  A non-empty body's
         * last node always JMPs to γ/ω explicitly (both defined after res), so this guard is n==0-only. */
        if (n == 0) emit_jmp_label(flat_empty_body_fail ? &lbl_ω : &lbl_γ, JMP_JMP);
        /* ZS-2 β-resume landing (Lon s58/s59): γ left a 16B resume record AT THE DEEP FRONTIER —
         * [rsp+0] = this landing's address (read, not popped, by the outside `jmp qword [rsp+0]`),
         * [rsp+8] = the suspended activation's frame reg (region base).  Interior suspended cells live
         * BELOW the header at suspension time, so the frontier — not the header — is the only address the
         * LIFO law hands back to β.  Drop the landing word, pop the frame reg, fall into the chain's
         * resume dispatch at lbl_β with rsp restored to the pre-suspension frontier. */
        emit_sep_rule('-'); emit_label_define_bb(&lbl_res);
        if (g_is_text) {
            char _res[96];
            snprintf(_res, sizeof _res, "add rsp, 8\npop %s\n", emit_rec_fb());   /* REG-7 U5: the record[+8] payload is the FRAME BASE — rbp for RSP pat blobs (Lon FORTH ruling; zr is sealing to rsp and can no longer name it), r12/rbp legacy modes byte-identical via the fb selector */
            emit_text_n(_res, strlen(_res));
        } else {
            int z = emit_rec_fb_num();
            ef_b4(0x48, 0x83, 0xC4, 0x08);
            if (z >= 8) ef_b2(0x41, (uint8_t)(0x58 | (z & 7))); else ef_b1((uint8_t)(0x58 | (z & 7)));
        }
    }
    emit_sep_rule('-'); emit_label_define_bb(&lbl_β);
    if (g_suspend_resume_slot >= 0 && emit_heap_fb_adopt()) {   /* ZETA-FB-3 (s161): the SEVENTH copy of the adopt condition, now named — this is the β frame-slot dispatch the emit_rec_pin comment calls out as "ALREADY gates on this pair".  ⚠ OBSERVED, NOT CHANGED: the GUARD is the NARROW adopt-only arm while the body spells emit_rec_fb()/emit_rec_fb_num(), the WIDE union — sound today because adopt ⟹ emit_rec_pin, so the register the body names is right whenever this arm is taken.  A graph that is emit_jmp_pin_rbp() but NOT adopt, carrying a resume slot, therefore takes the ELSE arm by construction; whether it SHOULD is a separate measurable question, and blind widening of exactly this kind is what s158 measured as a regression.  Named here, deliberately NOT widened. */
        if (g_is_text) {
            char _ind_jmp[64];
            snprintf(_ind_jmp, sizeof _ind_jmp, "jmp qword ptr [%s + %d]\n", emit_rec_fb(), g_suspend_resume_slot);   /* REG-7 U5: frame-slot dispatch via fb (rbp under RSP — re-pinned by the res landing before arrival) */
            emit_text_n(_ind_jmp, strlen(_ind_jmp));
        } else {
            { int z = emit_rec_fb_num(), lo = z & 7; ef_b2((uint8_t)(0x48 | (z >= 8 ? 0x01 : 0x00)), 0xFF); if (lo == 4) ef_b2(0xA4, 0x24); else ef_b1((uint8_t)(0xA0 | lo)); bb_emit_u32((uint32_t)(unsigned)g_suspend_resume_slot); }   /* REG-7 U5: fb twin */
        }
    } else {
        bb_label_t *resume_tgt = &lbl_ω;
        for (int i = 0; i < n; i++) if (nodes[i]->op == IR_SUSPEND) { resume_tgt = betas[i]; break; }
        if (g_emit_cfg && g_emit_cfg->body_root)
            for (int i = 0; i < n; i++) if (nodes[i] == g_emit_cfg->body_root && nodes[i]->op == IR_DISJUNCTION) { resume_tgt = lbls[i]; break; }
        if (g_emit_cfg && g_emit_cfg->body_root)
            for (int i = 0; i < n; i++) if (nodes[i] == g_emit_cfg->body_root && nodes[i]->op == IR_CALL_BUILTIN_GEN) { resume_tgt = betas[i]; break; }
        emit_jmp_label(resume_tgt, JMP_JMP);
    }
    if (g_emit.flat_stmt_frame) { emit_sep_rule('-'); emit_label_define_bb(&lbl_stcγ); bb_emit_x86(bb_glue_framed_leave()); emit_jmp_label(&lbl_γ, JMP_JMP); emit_label_define_bb(&lbl_stcω); bb_emit_x86(bb_glue_framed_leave()); emit_jmp_label(&lbl_ω, JMP_JMP); }   /* STMT-FRAME chain-exit cuts: leave (mov rsp,rbp; pop rbp) then jmp the port -- every retargeted γ/ω edge lands here; the cut normalizes rsp from ANY statement depth, which is what licenses leaving every template's inline pop untouched.  GLUE-4 (s21x-p): the cut is bb_glue_framed_leave -- takes no K by design, so it is byte-identical unconditionally. */
    emit_sep_rule('-'); emit_label_define_bb(&lbl_γ);
    xa_dispatch(XA_FLAT_EPILOGUE);
    /* PL-DC (REGAIN-1 SLICE C, s108): the per-proc DIRECT-CALL stub, appended after the shared exits when the driver armed the graph (g_flat_dc_np >= 0: registered det-lexical jmp-entry proc,
     * nparams<=4, hatch SCRIP_NO_DC unset — the SAME table facts the site predicate reads, so site and callee agree by construction).  Refinement guards here are LOUD: a driver-armed graph that
     * turns out not to be the lex jmp-entry shape is a predicate drift, not a codegen mode.  m3: the stub label's slab offset rides out in g_last_dc_off for the driver's rt_proc_set_dcfn(pfn+off)
     * registration at seal; m4: the named label is baked by the startup twin. */
    { extern int g_flat_dc_np; extern long g_last_dc_off; g_last_dc_off = -1;
      if (g_flat_dc_np >= 0) {
          if (!g_emit.flat_jmp_entry || !g_emit.flat_lex || g_emit.flat_gen || g_emit.flat_pat) { fprintf(stderr, "FATAL PL-DC: driver-armed graph is not det-lexical jmp-entry (jmp=%d lex=%d gen=%d pat=%d)\n", g_emit.flat_jmp_entry, g_emit.flat_lex, g_emit.flat_gen, g_emit.flat_pat); abort(); }
          bb_label_t lbl_dc; emit_label_initf(&lbl_dc, "%s_dcα", prefix);
          emit_sep_rule('-'); emit_label_define_bb(&lbl_dc);
          g_emit.flat_dc_body_p = &lbl_α_body;
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
        switch (n->op) { case IR_RETURN: case IR_SUSPEND: case IR_CORET: case IR_COFAIL: sp = 0; continue; default: break; }   /* control transfer is never a value: reset the sim stack, never ride it (mirrors ar<0) */
        int ar = emit_chain_arity(n);
        if (ar < 0) { sp = 0; continue; }
        else if (ar >= 1 && sp >= ar) { if (n->n_operands < ar) { n->n_operands = 0; for (int k = ar; k >= 1; k--) ir_operand_push(n, stk[sp - k]); } sp -= ar; }
        else if (ar >= 1) { sp = 0; }
        stk[sp++] = n;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ZS-2/PROC-CONV: derive the jmp-entry region from the graph and arm the self-allocating regime (flat_jmp_entry
 * + K_total = 32B wire header + zls region, 16-mult).  The emitted activation is IDENTICAL for every citizen —
 * PAT$ patprocs, EVAL/CODE chains, ordinary procs — so the three gates below differ ONLY in which they admit. */
static int emit_jmp_entry_arm_region(IR_graph_t *g) {
    extern int zls_g_region(const IR_graph_t *);
    extern int zls_g_resume(const IR_graph_t *);
    extern int zls_g_zeta_mark(const IR_graph_t *);
    int rg = g ? zls_g_region(g) : -1;
    int so = g ? zls_g_resume(g) : -1;
    if (so < 0 && g) so = zls_g_zeta_mark(g);
    if (so < 0 && g) { extern int zls_g_locals(const IR_graph_t *); so = zls_g_locals(g); if (so < 0) so = rg; }   /* ALIGN-INV-3c take 2: suffix = zls's own locals boundary (the old mark cursor, recorded unconditionally) -- NOT min(vslot), because params ARE vslots at +16 and prep binds BEFORE seeding (fact(n) regression: seed from +16 NULVCLed the just-bound n) */
    int capen; { static int v = -1; if (v < 0) { const char *e = getenv("SCRIP_CAP_NOFILL"); v = e ? (atoi(e) != 0) : 1; } capen = v; }   /* CAP-NOFILL (s143) kill-switch: 0 restores the s139 pull-down (SCRIP_PAT_NOFILL precedent) */
    g_emit.flat_cap_n = 0; int capovf = !capen;
    if (g) for (int i = 0; i < g->n; i++) { IR_t *nd = g->all[i]; if (nd && nd->op == IR_MATCH_ASSIGN_SAVE) { int off = nd_slot(nd); if (off < 0) continue;
        if (!capovf) { int dup = 0; for (int j = 0; j < g_emit.flat_cap_n; j++) if (g_emit.flat_cap_off[j] == off) { dup = 1; break; } if (dup) continue;
            if (g_emit.flat_cap_n < 48) { g_emit.flat_cap_off[g_emit.flat_cap_n++] = off; continue; }
            capovf = 1; for (int j = 0; j < g_emit.flat_cap_n; j++) if (so < 0 || g_emit.flat_cap_off[j] < so) so = g_emit.flat_cap_off[j]; g_emit.flat_cap_n = 0; }
        if (so < 0 || off < so) so = off; } }   /* SPD-NOFILL fix (s139) + CAP-NOFILL (s143): capture head cells (rt_cap_push/pop/top take &slot — empty state IS zero, read-before-first-push by design; the +0 buf ptr is deref'd after gen self-heal, so BOTH quads of the 16B grant must be zero) are implicit-zero citizens like resume/zeta-mark, NOT first-write-wins grants.  s139 pulled the zero suffix down to the lowest one, blanketing every grant above it (expression-eval PAT$: 560B rep stosb per activation = 52% of blob Ir); s143 records each site's offset in flat_cap_off for INDIVIDUAL zeroing by xa_flat's SPD-NOFILL arm, keeping flat_seed_off at the true resume/zeta-mark suffix.  SCRIP_CAP_NOFILL=0 or >48 distinct sites = the s139 pull-down.  s139 bisect provenance: 124 ([rbp+144]); explains 124/131/expr_eval/146.  Trio has no captures — no cost there. */
    g_emit.flat_layout_unknown = 0;
    if (rg <= 0) { rg = 4096; so = -1; g_emit.flat_layout_unknown = 1; }   /* SPD-NOFILL (s139): fallback graphs must keep the eager fill — see emit.h flat_layout_unknown */
    { extern int g_flat_frame_floor; if (g_flat_frame_floor > 0 && rg < g_flat_frame_floor) { rg = g_flat_frame_floor; so = -1; g_emit.flat_layout_unknown = 1; } }   /* SN4-FLAT-PROC (s176): a DEFINE stub's activation IS a fresh MAIN-layout frame — the body statements it transfers into address main's [rbp+off] slots, so the driver floors the stub's region at main's (zls_g_region on the shared graph, valid because drive_slots_all builds every graph's zls before any proc emits).  layout_unknown forces the eager full zero: main's implicit-zero suffix/capture citizens sit at MAIN offsets this stub's own tiny layout knows nothing about, and a fresh recursion frame must satisfy them all. */
    g_emit.flat_jmp_entry = 1; g_emit.flat_frame_bytes = (48 + (g_emit_cfg ? g_emit_cfg->jcon_value_region : 0) + 15) & ~15;   /* ⛔⭐⭐ CARVE-ERAD OVERREACH, CORRECTED (s22j).  This line read a HARD-CLAMPED `48` on the premise that "every BB now allocates its own storage at α through the ZW-1 hook, nothing is pre-allocated on behalf of anybody else".  THAT PREMISE IS FALSE FOR THE ZERO-CELL BOXES, and IR_MATCH_ALTERNATE is the witness: fc_geom DECLINES it, its own ALT-FLAT template header states "the box moves rsp nowhere", and its entire state is the FLAT zls quads FR(op_off)/FRQ(op_off+8)/FRQ(op_off+16).  A box that carves nothing keeps living in the graph slot region, so deleting that region did not free it -- it made the quads ALIAS the jmp-entry protocol's own save slots.  MEASURED on `PAT = ('aaa'|'bbb') ; S PAT`: kt=48 puts the protocol in the top 40 bytes (SCANBASE cursor kt-40, r8 kt-32, saved rcx/rdx/rbp kt-24/-16/-8) leaving 8 bytes for the graph, while the ALT's quads sit at op_off 16/24/32 -- landing exactly on r8, rcx and rdx.  Observed rbp=0x0 then an indirect jump through an rbp slot to address 0.  This is THE MODEL's own named failure mode ("DO NOT cut the sites before their readers are covered -- deleting the static authority under unconverted readers regresses programs"), and emit.h's flat_all_zd comment already states the rule this line broke: "jmp-entry blobs, flat_pat, flat_gen keep legacy verbatim (their protocols read the pinned whole-graph frame)".  jcon_value_region is still computed (emit.cpp:841/2673, xa_flat.cpp:131), so the region is restored rather than reinvented.  ⭐ 48 IS THE FLOOR, NOT THE FORMULA: the old spelling (32+rg+15)&~15 yields 32 at rg=0, which puts SCANBASE's kt-40 store at a NEGATIVE displacement; 48+rg keeps the wire header + 16B pad intact and is byte-identical to the clamp whenever rg==0.  The region shrinks on its own as kinds arm -- when the zero-cell families convert, rg goes to 0 and this returns to 48 without an edit here. */   /* ⭐ CARVE-ERAD (Lon directive: "remove the whole graph carve, we will not need any more").  WAS (32 + rg + 15) & ~15 -- 32B wire header PLUS rg = jcon_value_region, the WHOLE-GRAPH prefix-summed slot region.  rg IS the carve and it is DELETED: the residual 48 is the wire header (outside-γ at kt-24, outside-ω at kt-16, saved rbp at kt-8) plus 16B align pad, which is this box's OWN four-port protocol, not a graph frame -- law 1 permits a box a few bytes of its own.  Every BB now allocates its own storage at α through the ZW-1 hook and releases at γ/ω; nothing is pre-allocated on behalf of anybody else.  The old SCRIP_NO_GRAPH_CARVE=1 bisect flag simulated exactly this by clamping to 48 and is now the permanent and only behaviour, so it is deleted with the thing it clamped. */
    g_emit.flat_seed_off = (so >= 16 && so <= rg) ? so : 0;   /* PL-REGAIN-4: seed suffix start for the lazy lexprep2 prologue; 0 = fall back to full fill.  ALIGN-INV-3c: so == rg is a LEGAL EMPTY suffix (RSP reclaimed the mark; every consumer treats [rg,rg) as no-op) -- the old so+16<=rg guard collided empty with the 0 sentinel and full-seeded from 16, wiping bound params (fact(n) -> 1) */
    return 1;
}
/* ZS-2 (Lon s58/s59): lower-synthetic PAT$N pattern procs are consumed ONLY through SNO$MKPAT → DT_P → the
 * jmp-entry DEFER transfer ($ is not a SNOBOL4 identifier character, so no user code can call them), so their
 * chains must speak the jmp-entry protocol: self-allocating activation with the 32B wire header, wired
 * outside-γ/ω, jmp back — never call/ret.  The four driver proc-emission loops bracket PAT$ emissions with this
 * pair; ordinary procs join via emit_jmp_entry_for_proc once PROC-CONV (R12-FREE ladder rung 2) wires the site. */
extern "C" int emit_jmp_entry_for_patproc(const char *pname, IR_graph_t *g) {
    if (!pname || strncmp(pname, "PAT$", 4) != 0) return 0;
    g_emit.flat_pat = 1;   /* R12-ERAD s65: suspending blob — r12-island flavor */
    return emit_jmp_entry_arm_region(g);
}
int  g_flat_frame_floor = 0;   /* SN4-FLAT-PROC (s176): region-bytes floor for the NEXT jmp-entry arm — driver sets to main's zls region around DEFINE-stub emissions (see emit_jmp_entry_arm_region), clears after */
int  g_flat_dc_np = -1;   /* PL-DC (s108): driver-armed direct-call eligibility for the NEXT graph — nparams (0..4) when the callee passes the table predicate (registered, !dyn, !gen, np<=4, hatch off), -1 otherwise.  Set beside emit_jmp_entry_for_proc, cleared with the rest of the jmp-entry arm. */
long g_last_dc_off = -1;  /* PL-DC (s108): slab byte offset of the just-emitted graph's dc stub label (m3), -1 = none; the driver registers pfn+off via rt_proc_set_dcfn at seal. */
extern "C" int zop_audit_seen(void) { return g_emit.zop_seen; }   /* ZOP-1: the C-side reader of the regime mask, so the tally can live in zeta_storage.c beside the rest of the ζ bookkeeping rather than in the emitter. */
extern "C" void zop_audit_seen_clear(void) { g_emit.zop_seen = 0; }   /* ZOP-1: makes zop_audit_graph_close IDEMPOTENT, which is what lets the atexit path close the final graph without risking a double count of it. */
extern "C" void emit_jmp_entry_clear(void) { extern int g_flat_frame_floor; g_flat_frame_floor = 0; g_emit.flat_jmp_entry = 0; g_emit.flat_frame_bytes = 0; g_emit.flat_seed_off = 0; g_emit.flat_layout_unknown = 0; g_emit.flat_pat = 0; g_emit.flat_lex = 0; g_emit.flat_gen = 0; g_emit.flat_deep_arrival = 0; g_emit.flat_cap_n = 0; g_flat_dc_np = -1; }
/* PROC-CONV (R12-FREE ladder rung 2) + NCB-1d (Lon "RSP/RBP FORTH ζ for ALL, sharing the C stack", s90): ordinary procs arm the SAME jmp-entry regime.  DYN procs (SNOBOL4 DEFINE): args ride the name
 * dictionary, a self-allocated zeroed frame is correct.  DET LEXICAL procs (dyn_scope=0 — Icon/Raku): NCB-1d retires their call-regime trampoline — the blob self-allocates and its prologue calls
 * rt_jmp_frame_lexprep (NULVCL fill + rt_frame_bind_args from the staged g_call_args), so the caller-made-frame protocol is gone and flat_lex marks the graph for that prologue tail.  GENERATOR procs
 * are ADMITTED with flat_gen (GENP-SPINE s92): same lexprep prologue, but the γ epilogue RETAINS the activation on the spine (suspend record) — see emit.h flat_gen.  LBL__ main-program pseudo-procs are dyn_scope=0 but take no args and
 * are entered only through rt_chain_enter (rt_goto_transfer arm 4), which IS the jmp transfer — they arm with flat_lex=0 (no pcall record exists for lexprep to read). */
static int emit_graph_has_suspend(IR_graph_t *g) { if (!g) return 0; for (int i = 0; i < g->n; i++) if (g->all[i] && g->all[i]->op == IR_SUSPEND) return 1; return 0; }
static int emit_graph_has_deep_arrival(IR_graph_t *g) {   /* PIN-ALL RETIRED same session by FLATDISP-9 (Lon PIVOT: RSP zeta is the direction, rbp only for the intended frame constructs): the pin was the stopgap for the FR-vs-encoder double-add of op_flat_disp; with FR/FRQ/FRQB carrying RAW offsets the RSP regime is re-armed and re-measured.  Pin-all numbers preserved for the record: 313/2 · 311/2 · D0; re-enabling is one `return 1;` here.  MEASURED: crosscheck 304/11 · 303/10 · D3 with the census below vs 313/2 · 311/2 · D0 pinned -- the census-exempt RSP regime carried the FLATDISP class (rsp-relative FRQ with a baked op_flat_disp that over-counts cells popped before the reader; f3 witness: span read baked +128 against 32 live, so runtime-arg primitives read a virgin slot 72 bytes above their coerce), plus the 047/140/141 SEGV families and every DIVERGE.  rbp-relative FRQ is depth-immune, so the whole class retires; FORTH cells still carve and pop identically -- only the fragile addressing goes dormant.  The census below is PRESERVED as the re-earn path: re-narrowing is deleting this line, licensed only by an fc-prefix audit that makes baked disp equal live depth at every FRQ reader.  Aligned with REG-7 U4 (unconditional-rbp frame) and the rbp-pin-main rung. */ if (!g) return 1; for (int i = 0; i < g->n; i++) { IR_t *c = g->all[i]; if (!c) continue; switch (c->op) { case IR_SUSPEND: case IR_SCAN: case IR_SCAN_ENTER: case IR_SCAN_ALTERNATE: case IR_SCAN_SEQUENCE: case IR_SCAN_UPTO: case IR_SCAN_FIND: case IR_SCAN_BAL: case IR_SCAN_MATCH: case IR_SCAN_MOVE: case IR_SCAN_TAB: case IR_TO: case IR_TO_BY: case IR_LIMIT: case IR_REPALT: case IR_PROC_GEN: case IR_CREATE: case IR_ITERATE: case IR_DISJUNCTION: case IR_CALL_BUILTIN_GEN: case IR_KEYWORD_ICON_GEN: case IR_MATCH_FENCE1: case IR_MATCH_ABORT: case IR_MATCH_ARBNO: case IR_MATCH_CALLOUT: case IR_MATCH_VALUE: return 1; case IR_MATCH_DEFER: case IR_MATCH_PATREF: return 1;   /* PATCTX (2026-07-29): the is_star gate DELETED -- census autopsy on 125_pat_json_literal: a NON-star sealed defer's γ-continuation arrives at blob-residue depth (PAT$γ pushes rbp+res and jumps with rsp 16+blobframe LOW), yet its stamp restore is spelled FRQ(op_off), whose depth-immunity the s137 comment itself conditions on rbp ("FRQ(op_off), rbp-relative → recursion-safe").  In the RSP regime that read only ever worked by SLOT-VALUE ALIASING: [rsp+mark] at residue depth landed on head.zls2_mark (+16), which holds the same rsp value -- the PATCTX grant widening moved the alias target onto head.sigma_save (outer r13 == 0 at the outermost match) and the program died at rsp=0, exposing the class.  Star was never the load-bearing property; γ-retention is, and every defer target can retain.  Same conservatism rule as the rest of the list: completeness IS the safety. */ default: break; } } return 0; }   /* FLATDISP-6 (s193, Lon's FOUR-CONSTRUCT ruling: STATEMENT/FUNCTION/ARBNO/FENCE1): the list is the rbp-READER set established by template census — FENCE1 reads rbp as the dynamic-ζ floor (mov rsp,rbp + watermark := rbp); ARBNO BORROWS rbp as zv() the element-view register (bb_match_arbno 222/229/242 re-point it — Lon's "housekeeping about variable length children based off of RBP", and WHY head/release/replace carry the +40 save/restore bracket); ABORT bypasses the β pop-cascade so its kill lands with pattern cells still carved (dynamic depth) — VISIBLE since ABORT-NODE (this session) relowered TT_ABORT from a bare IR_GOTO to the real IR_MATCH_ABORT; DEFER/VALUE σ-transfer into γ-retaining PAT$ blobs (post-transfer depth dynamic, the FORTH-ledger depth-immune watermark fact); CALLOUT conservative.  HEAD REMOVED (s193, the ABORT-NODE unlock): a fence/arbno/abort/defer-free match statement is depth-static — the match protocol rebalances rsp through the head snapshot at BOTH exits (058 .s ground truth), and head/release/replace's rbp instructions are save/restore-of-caller-value, value-neutral without a seed.  HEAD's earlier retention was measured, not ceremonial: dropping it BEFORE the abort node existed cost exactly 170/171 (m4 183→181→183, tripwire proven both directions).  Also dropped s193, subsumed then and free now: ALTERNATE, SEQUENCE, ARB, RETRY, RELEASE (comment-only in census).  FUNCTION is covered by the jmp-entry arm (seeds unconditionally, both media).  ⚠ COMMENT CORRECTION (prose-rot, RULES.md (a)-shape): the s192 text here claimed "every unlisted-but-present kind falls through to the SAFE answer (1)" — FALSE: default: break falls to return 0; the conservatism lives in the LIST being complete, which is why the census had to be by-template-read.  The crosscheck watermark is the tripwire, twice proven this session. */
static IR_t * g_fbm_nd[8192]; static unsigned char g_fbm_bit[8192]; static int g_fbm_n = 0;
static int emit_fb_bit_of(IR_t * nd) { for (int i = 0; i < g_fbm_n; i++) if (g_fbm_nd[i] == nd) return g_fbm_bit[i]; return 1; }   /* FB-STMT: unmapped node (foreign driver) stays rbp — conservative */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int emit_fb_stmt_scan(IR_graph_t * g) {   /* FB-STMT (Lon 2026-07-29): eligibility + per-node bit map.  Eligible iff every deep kind present is statement-bracketed (DEFER/ARBNO/FENCE1/VALUE); any suspend-class, generator-class, ABORT or CALLOUT kind disqualifies the whole graph (their arrivals are not rebalanced by the HEAD..RELEASE bracket).  Bits: every match-family op = 1 (pattern boxes and their quads are the housekeeping rbp set); every node inside a HEAD..RELEASE/REPLACE range CONTAINING a deep kind = 1 (pre-chains at match depth included by position); everything else = 0.  An unterminated HEAD disqualifies — degrade never die. */
    static int fbdbg = -1; if (fbdbg < 0) { const char * e = getenv("SCRIP_FB_DEBUG"); fbdbg = (e && *e == '1') ? 1 : 0; }
    g_fbm_n = 0; if (!g || g->n <= 0 || g->n > 8192) { if (fbdbg) fprintf(stderr, "[FB-STMT] bail size g=%p n=%d\n", (void *)g, g ? g->n : -1); return 0; }
    for (int i = 0; i < g->n; i++) { IR_t * c = g->all[i]; if (!c) continue; switch (c->op) {
        case IR_SUSPEND: case IR_SCAN: case IR_SCAN_ENTER: case IR_SCAN_ALTERNATE: case IR_SCAN_SEQUENCE: case IR_SCAN_UPTO: case IR_SCAN_FIND: case IR_SCAN_BAL: case IR_SCAN_MATCH: case IR_SCAN_MOVE: case IR_SCAN_TAB:
        case IR_TO: case IR_TO_BY: case IR_LIMIT: case IR_REPALT: case IR_PROC_GEN: case IR_CREATE: case IR_ITERATE: case IR_DISJUNCTION: case IR_CALL_BUILTIN_GEN: case IR_KEYWORD_ICON_GEN:
        case IR_MATCH_ABORT: case IR_MATCH_CALLOUT: { if (fbdbg) { extern const char * bb_op_name(IR_e); fprintf(stderr, "[FB-STMT] bail kind %s @%d n=%d\n", bb_op_name(c->op), i, g->n); } return 0; }
        default: break; } }
    for (int i = 0; i < g->n; i++) { IR_t * c = g->all[i]; g_fbm_nd[g_fbm_n] = c; g_fbm_bit[g_fbm_n] = (c && c->op >= IR_MATCH_LIT && c->op <= IR_MATCH_ADVANCE) ? 1 : 0; g_fbm_n++; }
    for (int h = 0; h < g->n; h++) { IR_t * hn = g->all[h]; if (!hn || hn->op != IR_MATCH_HEAD) continue; int e = -1, deep = 0;
        for (int k = h + 1; k < g->n; k++) { IR_t * c = g->all[k]; if (!c) continue;
            if (c->op == IR_MATCH_DEFER || c->op == IR_MATCH_PATREF || c->op == IR_MATCH_ARBNO || c->op == IR_MATCH_FENCE1 || c->op == IR_MATCH_VALUE) deep = 1;
            if (c->op == IR_MATCH_RELEASE || c->op == IR_MATCH_REPLACE) { e = k; break; } }
        if (e < 0) { if (fbdbg) fprintf(stderr, "[FB-STMT] bail unterminated HEAD @%d n=%d\n", h, g->n); return 0; }
        if (deep) { for (int k = h; k <= e; k++) g_fbm_bit[k] = 1; } }
    if (fbdbg) { int z = 0; for (int i = 0; i < g_fbm_n; i++) if (!g_fbm_bit[i]) z++; fprintf(stderr, "[FB-STMT] refine=1 n=%d rsp_nodes=%d\n", g_fbm_n, z); }
    return 1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int emit_stmt_frame_scan(IR_graph_t * g, IR_t * entry) {   /* STMT-FRAME classifier, SLICE 1 (s21x-c NEXT EXECUTABLE: "straight-line int-only statement graphs first"): ADMIT iff every node REACHABLE from entry is in the closed whitelist {IR_LIT_INTEGER, IR_LIT_STRING, IR_VAR, IR_BINOP, IR_ASSIGN} (IR_LIT_STRING joined at the s21x-e string rung: registry-symmetric per ZB-VAL-6a, one bb_lit_scalar body, concat trees ride the producer conjunct transitively).  Reachability, not g->all: lower seeds inert per-graph companions (two IR_SAVE_RESTORE floaters + an IR_CALL/IR_LIT_STRING NRETURN pair, measured on the sl0 witness) that never reach the emitted chain -- name-testing them here would be a language sentinel (BANNED past first dispatch), so the license is scoped to what will EMIT.  The walk mirrors the codegen chain BFS restricted to whitelist ops (γ always; ω for the BINOP/VAR classes, the real BFS's rule for exactly these kinds); the mirror is a safe superset BY CONSTRUCTION because a reached non-whitelist op rejects before its unknown edge classes matter.  GOTO LAW (s21x-e, measured on 031/036/037 + the sw3 falsifier): unconditional gotos arrive FOLDED to bare edges (optimizer GOTO-chase), so they pass the kind walk invisibly -- slice 1's "no gotos admitted" was the string-lit rejection MASKING them, never a checked invariant.  Edges into NON-first heads are correct by the st_x leave-then-enter cut (retargets need no membership map); edges into ENTRY are rejected inline below because the first head is st_x-exempt.  The list GROWS one construct per rung; whitelist completeness IS the safety (the FLATDISP-6 conservatism rule). */
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
        if ((c->γ.node && c->γ.node == entry) || (c->ω.node && c->ω.node == entry)) { if (_sd) fprintf(stderr, "[STF] reject BACKWARD EDGE TO FIRST HEAD sn=%d\n", sn); return 0; }   /* GOTO LAW (s21x-e, sw3 falsifier): the first head is st_x-exempt (prologue falls in bare), so an edge targeting ENTRY enters a bracket without leaving one -- 16B leak per arrival.  Non-first heads are served by the st_x leave-then-enter cut and stay admitted. */
        switch (c->op) { case IR_LIT_INTEGER: case IR_LIT_STRING: case IR_ASSIGN: break;
            case IR_VAR: case IR_BINOP: if (c->ω.node) q[qt++] = c->ω.node; break;
            default: if (c->ω.node) q[qt++] = c->ω.node; break; }   /* ACROSS-THE-BOARD (s21x-e, Lon: "add the RSP decrement across the board not one at a time… would not be prudent") -- the kind whitelist DIES: every construct is admitted and the corpus names the brick walls by CLASS, per the s21x-c method ("proceed RSP-only for EVERY box and EVERY construct until a construct hits a BRICK WALL").  Both edges enqueued for unknown kinds (safe superset walk).  The remaining rejects are the MEASURED ones only: the fc-flavor producer conjunct below (026/027, flat-slot readers presume the graph carve) and the entry-edge GOTO LAW above.  Guard conjuncts at the choke (jmp-entry/pat/gen/resumable/nparams) still fence the suspension-protocol and call-protocol families -- those are the FUNCTION-construct and blob rungs, not kind gates. */
        { extern int fc_geom(const IR_t *, long *); long _fk = 0; int _fc = fc_geom(c, &_fk); if (_sd) { extern const char * bb_op_name(IR_e); fprintf(stderr, "[STF] node %s fc=%d k=%ld\n", bb_op_name(c->op), _fc, _fk); }
          static int _c2c = -1; if (_c2c < 0) { const char * e2 = getenv("SCRIP_CALL2BB"); _c2c = (e2 && *e2 == '1') ? 1 : 0; }
          int _callfam = (_c2c && (c->op == IR_CALL || c->op == IR_CALL_PROC_STAGED || c->op == IR_SAVE_RESTORE || c->op == IR_GOTO_DEFERRED)) ? 1 : 0;   /* + IR_GOTO_DEFERRED: a control-transfer SINK like IR_GOTO -- it jmps away through rt_goto_transfer/the registry and never returns to this statement, and every landing protocol it can reach (floaters' record restore, chain-enter) normalizes rsp/rbp wholesale, so abandoned bracket depth is by-design (the same argument as the floaters' discarded-C-frames note in bb_save_restore.cpp). */   /* CALL2BB slice 3: the FUNCTION-construct admit (the s21x-e NEXT (b) rung) -- all three are self-allocating by construction (sr0 carves/releases its own save block per slice 2; the staged call box keeps rsp balanced on every arm and its exits ride the record protocol; floaters restore machine state wholesale from the record), so fc=0 on them is their FORTH form, not a flat-slot read.  Gated on SCRIP_CALL2BB so the STMT_FRAME-only world is byte-identical to s21x-e. */
          { extern int fc_vread_fp(const IR_t *); if (!_fc && c->op == IR_MATCH_HEAD && fc_vread_fp(c) >= 0) _fc = 1;
            if (!_fc && c->op == IR_MATCH_RELEASE && c->n_operands > 0 && c->operands[0] && fc_vread_fp(c->operands[0]) >= 0) _fc = 1;   /* SUBJ-ARM-1: RELEASE licensed iff ITS statement's head is subject-cell licensed -- one authority; its frame reads are the head-field pair (window via op_fc_disp; PATCTX flat quartet written by head at the SAME post-unwind depth it reads them) */
            if (!_fc && c->op == IR_MATCH_LIT) _fc = 1; }   /* SUBJ-ARM-1: MATCH_LIT is ZERO frame refs by template inspection (register+RO compare only, no FR/FRQ in bb_match_lit.cpp) -- the zero-cell-by-law exempt set growing by measurement, the Icon ANY/POS precedent this comment block anticipated */   /* SUBJECT-CELL rung (a): HEAD is fc_geom 0 BY LAW (self-releasing 32B window, never summed) so its license under the regime is the fvr read registration -- the SAME registry the drive-case promotion reads, planner and classifier one authority */
          if (!_fc && !(c->op == IR_BINOP || c->op == IR_ASSIGN || c->op == IR_GOTO || _callfam)) { if (_sd) { extern const char * bb_op_name(IR_e); fprintf(stderr, "[STF] reject UNREGISTERED %s sn=%d\n", bb_op_name(c->op), sn); } return 0; } }   /* ACROSS-THE-BOARD conjunct (s21x-e, inverted polarity): under admit-all-kinds the producer LIST dies too -- ANY reachable fc=0 node rejects UNLESS it is a measured pure-TOS consumer (BINOP/ASSIGN, proven by the armed 28 incl. concat's str_concat_d C-call) or inert wiring (GOTO, zeta_storage 923).  The 097 autopsy generalized 026/027: IR_KEYWORD_SNOBOL4 / IR_CALL producers slipped a LIT/VAR-only conjunct and SEGV'd m4 identically (flat-slot arm presumes the graph carve).  END STATE: zeta_storage's fc registration is the ONE admission authority; widening the planner arms families here automatically, zero further classifier edits -- Lon's "not one at a time" delivered at the classifier.  Zero-cell-by-law kinds (the Icon ANY/POS precedent: fc_geom 0 IS their FORTH form) land conservative-reject until the exempt set grows by measurement. */   /* STMT-FRAME license conjunct 2 (026/027 root cause, this session): the whitelist admits the KIND; zeta_storage's fc registry independently picks the FLAVOR (the s191 FORTH registration -- fc_geom is the authority, final at this point because emit_chain runs zls_fct_finalize first).  A value-PRODUCER outside the registry takes the flat-slot template arm, whose [rsp + granted_off] addressing presumes the graph carve's region -- under the regime offset 40 at bracket depth is B+24 = THE CALLER'S RETURN ADDRESS, and the lit store shot it (026 m4 SEGV; m3 survived on entry-layout luck only).  Consumers (BINOP/ASSIGN) ride TOS and carry no own cell; requiring fc on them over-rejects (first fix attempt, measured on sl1).  The gate must read what the emitter will actually DO -- the FLATDISP-5b lesson again. */
        if (c->γ.node) q[qt++] = c->γ.node; }
    if (_sd) fprintf(stderr, "[STF] ADMIT reachable=%d (g->n=%d)\n", sn, g->n);
    return sn > 0 ? 1 : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int emit_graph_zstatic(IR_graph_t *g) { if (!g) return 0; for (int i = 0; i < g->n; i++) { IR_t *c = g->all[i]; if (c && (c->op == IR_MATCH_DEFER || c->op == IR_MATCH_PATREF || c->op == IR_MATCH_VALUE)) return 0; } return 1; }   /* PS-1b (s151): exact emit-side mirror of runtime bb_graph_zstatic -- a DEFER/VALUE node is the only transfer into an arriving blob of unknown extent, so its absence is what makes the frame sound for ARBNO stride arithmetic; the region-known conjunct is applied at the capture site via flat_layout_unknown. */
static int emit_graph_uniform(IR_graph_t *g) { if (!emit_graph_zstatic(g)) return 0; for (int i = 0; i < g->n; i++) { IR_t *c = g->all[i]; if (c && c->op == IR_MATCH_ARBNO) return 0; } return 1; }   /* PS-3 (s152): the STRICT ζ-uniformity predicate -- an interior ARBNO retains a VARIABLE count of element carves across γ-suspension (measured design fact, t1.s: interior cells stay carved on the hit path), so its presence breaks the constant-footprint claim even when zstatic==1.  zstatic gates the STAMP; uniform gates the STRIDE. */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" int emit_jmp_entry_for_proc(const char *pname, int dyn_scope, int is_generator, IR_graph_t *g) {
    if (pname && strncmp(pname, "gram__", 6) == 0) return 0;   /* NCB-1d EXCLUSION: Raku grammar boxes speak the SCAN-ENTRY protocol (rk_gram_enter_box trampoline — subject triad r13/r14/r15, esi entry selector, call/ret, spec_t in rax:rdx), not the proc-call protocol; they keep the outermost call-regime body.  Name-keyed like the LBL__ gate below. */
    int is_lbl = pname && strncmp(pname, "LBL__", 5) == 0;
    g_emit.flat_lex = (!dyn_scope && !is_lbl) ? 1 : 0;
    g_emit.flat_gen = (is_generator && emit_graph_has_suspend(g)) ? 1 : 0;   /* GENP-SPINE (s92): generators ADMITTED onto the main ζ spine — same self-allocating lexprep prologue as det procs, but xa_flat's γ epilogue RETAINS the activation (suspend record at the deep frontier) and the chain's resume-slot dispatch serves multi-site suspend re-entry; see emit.h flat_gen.  SUSPEND-PRESENCE GUARD: the retaining epilogue is meaningful only when a resume slot gets armed, and Icon's is_generator ⇔ body_has_suspend makes the guard a no-op there — it exists for the OTHER marker (lower_prolog sets is_generator=1 on suspend-free graphs): those keep the s91-parity det epilogue rather than retaining frames toward callers that never speak the record protocol. */
    return emit_jmp_entry_arm_region(g);
}
/* s60 EVAL/CODE-RSP (Lon ruling s59: EVAL/CODE = the DEFER shape): runtime-compiled EVAL statement chains and
 * CODE fragments speak the same jmp-entry protocol — self-allocating rsp activation, 32B wire header, wired
 * outside-γ/ω, jump back, never call/ret.  runtime_eval.c brackets its emit_chain calls with this pair and
 * enters through rt_chain_enter (the C-side resolve→wire→jmp).  Same K_total formula as the PAT$ gate above. */
extern "C" int emit_jmp_entry_for_chain(IR_graph_t *g) {
    extern int g_flat_dc_np; g_flat_dc_np = -1;
    return emit_jmp_entry_arm_region(g);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
bb_box_fn emit_chain(IR_t *entry, FILE *out, const char *prefix) {
    if (!entry) return NULL;
    emit_chain_operand_refs(entry);
    if (g_emit_cfg) zls_fct_finalize(g_emit_cfg, 1);   /* PS-3 s153 LATE fct finalize: defer-bearing tail candidates were left PENDING by the early pass (drive_slots_all runs before any proc registers its patzeta ζ terms); HERE every earlier-emitted PAT$ is registered, so the SUSP license resolves -- price or decline-to-chain -- before this graph's first node emits (fc_tail_head is consulted at IR_MATCH_HEAD). */
    g_bb_slotmap_n = 0;
    g_flat_chain_set_n = 0;
    { extern void bb_scc_handoff_reset(void); bb_scc_handoff_reset(); }   /* CALL2BB: per-graph handoff hygiene */
    { extern int g_scan_regs_live; g_scan_regs_live = 0; }   /* ICN-SCAN-SUSPEND-SYNC: scan regions are intragraph; without an emitted IR_SCAN leave box the flag leaked into later graphs, emitting sync brackets at non-scan call sites (r15.s main α garbage publish) */
    { extern void zop_audit_graph_close(void); zop_audit_graph_close(); g_emit.zop_seen = 0; }   /* ZOP-1 MIX AUDIT (Lon s21x-n): close the PREVIOUS graph's regime tally and re-arm.  This choke is where every other per-graph classification already lands, so the audit sees exactly the same graph boundary the pin/refine decisions do -- no separate notion of "graph" to drift out of sync. */
    g_emit.flat_all_zd = 0;   /* LP-1: cleared at the choke; codegen_flat_chain_body sets it from the pre-prologue kind scan */
    g_emit.flat_deep_arrival = emit_graph_has_deep_arrival(g_emit_cfg);   /* FLATDISP-5b (s193): classification moved HERE — FB-STMT rides the same choke point, next line */
    { int _rc_own = (g_emit_cfg && g_emit_cfg->resumable_callable) ? 1 : 0; g_emit.flat_fb_refine = (g_emit.flat_deep_arrival && !g_emit.flat_pat && !g_emit.flat_gen && !_rc_own) ? emit_fb_stmt_scan(g_emit_cfg) : 0;   /* FB-STMT guard reads THE GRAPH'S OWN resumable field, not g_resumable_callable_active — that global is refreshed only at jmp-entry arm (emit.cpp ~1821), so an outer main emitted after a PAT$ blob inherits the blob's 1 (measured on w1: hook skip rc=1 while mixed's RSUM proc happened to reset it).  The leak is benign for pinning (mains are deep anyway, emit_rec_pin unchanged) but starved the refinement; the stale-global class gets its own rung.  g_gen_proc_active dropped from the guard for the same reason — flat_gen (per-emission, cleared with flat_jmp_entry) is the honest generator signal at this choke point. */
      { static int fbdbg2 = -1; if (fbdbg2 < 0) { const char * e = getenv("SCRIP_FB_DEBUG"); fbdbg2 = (e && *e == '1') ? 1 : 0; } if (fbdbg2 && !g_emit.flat_fb_refine) fprintf(stderr, "[FB-STMT] hook skip deep=%d pat=%d gen=%d gp=%d rc=%d n=%d\n", g_emit.flat_deep_arrival, g_emit.flat_pat, g_emit.flat_gen, g_gen_proc_active, g_resumable_callable_active, g_emit_cfg ? g_emit_cfg->n : -1); } }   /* FB-STMT (Lon 2026-07-29): per-node data-ref refinement licensed only for statement-bracketed pins; suspend/heap-fb regimes keep whole-graph rbp.  Unpinned graphs need no map (x86_fb_data short-circuits on the pin). */
    /* FLATDISP-5b continued (s193): classification moved HERE from emit_jmp_entry_for_proc — the WRONG-GRANULARITY of s192 was not the kind list but the ARMING COVERAGE: only jmp-entry procs were ever classified, while the graphs whose prologue actually READS the gate (outer call-regime mains at xa_flat's g_frame_active arm, PAT$ blobs via emit_jmp_entry_for_patproc, EVAL/CODE chains, rt pattern builds) inherited the cleared 0 and dropped the rbp seed their own fence/scan boxes then read (058: main seeds nothing, n11_match_fence1 does `mov rsp, rbp` into the CRT frame).  emit_chain is the ONE choke point every graph emission passes, and every caller — driver loops, runtime_eval.c, bb_pat_build.cpp — sets g_emit_cfg to the graph immediately before, so this is the single writer; a null cfg falls through to the predicate's conservative 1.  Per-emission overwrite makes leaks from hand-armed callers (bb_pat_build restores fields manually, no emit_jmp_entry_clear) structurally impossible. */
    { static int _stf = -1; if (_stf < 0) { const char * e = getenv("SCRIP_STMT_FRAME"); _stf = (e && *e == '1') ? 1 : 0; }   /* ⛔⭐⭐ STF-UNFLIP (s22b, LON DIRECTIVE, and it is a MODEL correction not a tuning knob): DEFAULT FLIPPED BACK ON -> OFF; `SCRIP_STMT_FRAME=1` is now the OPT-IN and the regime ships DORMANT.  THE MODEL, restated in Lon's words because two sessions in a row encoded the opposite: NO BB EVER CONSUMES ANYTHING, EVER.  A BB allocates EXACTLY at alpha -- one `sub rsp, K`, its OWN cell -- never at beta (ARBNO-class excepted); it frees at omega; and the whole accumulation is freed AT THE END OF THE STATEMENT.  Cells stack up through the statement and ONE statement-terminal release drops all of them.  That release is op_zgpop, and it is the SOLE release authority.  ⛔ THE ERROR s21x-y AND s22a ENCODED: the per-statement rbp dance (bb_glue_framed_enter/leave) was made to stand in for that release, and s22a's ZGPOP-STF then ZEROED op_zgpop on armed graphs to stop the two colliding -- i.e. the bracket did not SUPPLEMENT the statement-terminal free, it REPLACED it, and a C-style frame pointer became per-statement ceremony on the value spine.  Law 4's "C-style RBP used occasionally" licenses rbp for CONSTRUCT housekeeping that must survive an unwind (ARBNO/FENCE1/CALL) -- it never licensed one bracket per statement.  MEASURED AT THIS HEAD, not inherited: crosscheck 318 m3 232/85 - m4 229/86/2 - DIV=1 {W04_arbno_basic}, IDENTICAL to the regime-on run, and the fail set is IDENTICAL BY SET in BOTH modes (diffed programmatically, not by count -- the test_string flake makes counts lie).  ⚠ NOT VACUOUS, and this is the instrument that proves it: an rbp-INSTRUCTION census over all 318 crosscheck programs moves 15709 -> 15459 (250 instructions gone) and the count of programs containing ANY rbp moves 158 -> 127, i.e. EXACTLY 31 PROGRAMS GO RBP-FREE -- the same 31 the SCRIP_STF_DEBUG=1 armed census names.  Witness `arithmetic.sno`: 26 rbp -> 0, statement `OUTPUT = A + B` now three alpha carves (`sub rsp,16` x3) and one terminal `add rsp,48`.  ⭐ THE REMAINING RBP DEBT IS A DIFFERENT CITIZEN AND IS THE NEXT RUNG: 15459 instructions across 127 programs is the flat_pat/flat_gen WHOLE-GRAPH PIN, not a statement bracket -- do not confuse the two (the s22a CENSUS-CORRECTED instrument law).  HISTORICAL, kept because the next session will otherwise re-derive it: s21x-y's flip was licensed by a watermark-neutrality argument that was TRUE and INSUFFICIENT -- neutrality proved the bracket broke nothing, never that it belonged. */   /* ⭐ STF-FLIP (s21x-y), SUPERSEDED BY THE ABOVE, retained for provenance: DEFAULT FLIPPED OFF -> ON, and `SCRIP_STMT_FRAME=0` is now the killswitch.  This is the moment the "C-style RBP used occasionally" of the design of record stops being dormant scaffolding and becomes the shipped regime: under it a STATEMENT is bracketed by the rbp dance (bb_glue_framed_enter/leave) and bb_match_head's five POST-UNWIND-lifetime fields move from the whole-graph flat slots FRQ(op_off+40..72) onto HKQ = [rbp-48+8k], which is DEPTH-FREE BY CONSTRUCTION -- the value spine keeps riding RSP FORTH-style while exactly the housekeeping that must survive an unwind rides a stable base.  LICENSED BY THE CONDITION THIS LINE ITSELF NAMED ("default OFF until the watermark license ... fail-set diffed programmatically"), now satisfied and MEASURED AT THIS HEAD, not inherited: crosscheck 317 m3 232/85 - m4 229/86/2 - DIV=1 and broad 336 m3 244/92 - m4 238/90/8, and in BOTH corpora BOTH modes the fail set is BYTE-IDENTICAL to the regime-off run.  ⚠ NOT VACUOUS -- the trap that makes an identical watermark meaningless: the regime ARMS 31 GRAPHS IN 31 PROGRAMS (SCRIP_STF_DEBUG=1, live=1 census), so the neutrality is 31 armed customers agreeing, not a classifier declining everything.  s21x-r's 41-program regression is NOT being re-litigated here: it was ZLEAK-1, a process-scope `static int` from getenv applied to all 316 programs while emit armed 31, cured at SCRIP `efb72d69`; the armed set and that regression set were measured DISJOINT then, and this re-measurement at a later HEAD confirms the cure held. */
      extern int g_flat_outer_nparams; extern int g_flat_frame_floor;
      int _c2g; { static int _c2 = -1; if (_c2 < 0) { const char * e = getenv("SCRIP_CALL2BB"); _c2 = (e && *e == '1') ? 1 : 0; } _c2g = _c2; }
      int _stfj = (_stf && _c2g && g_emit.flat_jmp_entry && !g_emit.flat_pat && !g_emit.flat_gen && !g_emit.flat_deep_arrival && !g_gen_proc_active && !(g_emit_cfg && g_emit_cfg->resumable_callable) && g_flat_frame_floor > 0) ? 1 : 0;   /* DEFINE-STUB DISCRIMINATOR (measured this session, two falsifications deep: admitting ALL jmp-entry citizens regressed expr_eval/161/1016/1019 -- runtime EVAL/CODE chains speak their own enter/exit protocol, not the pcall record's; keying on the emit_chain entry node's role-3 shape then missed the m4 driver path, whose entry parameter is not the wire-adopt node).  g_flat_frame_floor > 0 is the driver's OWN stub verdict: all four proc-emission loops (scrip.c 882/1340/1528/1633) set it from the role-3-entry predicate exactly and only around DEFINE-stub emissions, it is still live at this choke (emit_jmp_entry_clear runs after), and every other jmp-entry citizen -- PAT$ patprocs, EVAL/CODE chains, rt pattern builds -- leaves it 0.  Conservative edge: an empty-main program floors 0 and declines, which is the safe direction. */   /* CALL2BB slice 3 (Lon s21x-c laws 5/6/7: DEFINE constant-folded = IR_SAVE_RESTORE + IR_CALL; the whole-graph carve is the NAMED ANTI-PATTERN; statement scope is the planning ceiling): a jmp-entry DEFINE/LBL__ blob whose reachable body passes the SAME classifier rides the statement regime -- body statements self-allocate per-BB, so the blob needs NO region, NO fill, NO main-layout floor.  deep/pat/gen keep legacy verbatim (their record/suspend protocols read the pinned whole-graph frame). */
      g_emit.flat_stmt_frame = ((_stf && !g_emit.flat_jmp_entry && !g_emit.flat_pat && !g_emit.flat_gen && !g_gen_proc_active && !(g_emit_cfg && g_emit_cfg->resumable_callable) && g_flat_outer_nparams == 0) || _stfj) ? emit_stmt_frame_scan(g_emit_cfg, entry) : 0;
      { static struct { const void * g; const void * e; int v; } _sv[1024]; static int _sn = 0; static int _strict = -1; if (_strict < 0) { const char * e = getenv("SCRIP_STF_STRICT"); _strict = (e && *e == '1') ? 1 : 0; } if (_stf && g_emit_cfg) { int _f = -1; for (int i = 0; i < _sn; i++) if (_sv[i].g == (const void *)g_emit_cfg) { _f = i; break; }
        if (_f < 0) { if (_sn < 1024) { _sv[_sn].g = (const void *)g_emit_cfg; _sv[_sn].e = (const void *)entry; _sv[_sn].v = g_emit.flat_stmt_frame; _sn++; } }
        else { if (_sv[_f].v != g_emit.flat_stmt_frame) { fprintf(stderr, "WARN STF-STABILITY: graph %p verdict %d then %d -- the per-graph verdict is NOT stable across emissions; ZLEAK-3's plan-time fixpoint would mis-decide here (witness: 1014_func_freturn under CALL2BB)\n", (const void *)g_emit_cfg, _sv[_f].v, g_emit.flat_stmt_frame); if (_strict) abort(); }
              if (g_emit.flat_stmt_frame && _sv[_f].e != (const void *)entry) { fprintf(stderr, "WARN STF-SINGLE-ENTRY: ARMED graph %p emitted from entries %p and %p -- arming-implies-single-entry violated\n", (const void *)g_emit_cfg, _sv[_f].e, (const void *)entry); if (_strict) abort(); } } } }   /* ZLEAK-3 INVARIANT GUARD (s21x-r): the two premises the plan-time register/scan/unregister fixpoint rests on, MEASURED over the 317-program corpus and now ENFORCED rather than assumed -- (1) VERDICT STABILITY: a graph gets the same verdict at every emission (0 violations / 32 multi-choke graphs); (2) ARMING IMPLIES SINGLE ENTRY: every armed graph is emitted from exactly one entry (44 decisions / 44 distinct armed graphs, 1:1), while multi-entry graphs -- up to 11 entries, all DEFINE/indirect-goto/EVAL shapes -- decline at every entry.  Together these are what let ONE plan-time decision match EVERY emission: the planner scans from g->entry, and for any graph it would arm that entry is the only one, while a graph it declines is declined at all of them.  The strong premise the s21x-r cursor first wrote (plan-time entry == emit-time entry) is FALSE and is NOT what the fixpoint needs.  Regime-gated on _stf so the default path never executes this.  If either abort ever fires, ZLEAK-3 must be re-derived before it is trusted -- that is the whole point of spending four lines here. */
      if (g_emit.flat_stmt_frame && _stfj) { g_emit.flat_frame_bytes = 48; g_emit.flat_seed_off = 0; g_emit.flat_layout_unknown = 0; g_emit.flat_cap_n = 0; }   /* CALL2BB slice 3 kt override: 32B wire header + slot0 only (xa_flat's >=48 invariant is the floor) -- retires the s176 main-region floor (roman's 1344).  Every wire park/read and both epilogue arms are kt-parametric (flat_frame_bytes is the single source), so 48 is consistent at the prologue, the role-3 wire-adopt, and the determinate exits; the RETURN/FRETURN floaters restore the CALLER-side record rsp/rbp wholesale, abandoning this 48B header by design. */
      { static int _sd2 = -1; if (_sd2 < 0) { const char * e = getenv("SCRIP_STF_DEBUG"); _sd2 = (e && *e == '1') ? 1 : 0; } if (_sd2 && _stf) fprintf(stderr, "[STF] choke stf=%d jmp=%d pat=%d gen=%d gp=%d rc=%d np=%d deep=%d floor=%d c2=%d stfj=%d -> live=%d g=%p entry=%p\n", _stf, g_emit.flat_jmp_entry, g_emit.flat_pat, g_emit.flat_gen, g_gen_proc_active, (g_emit_cfg && g_emit_cfg->resumable_callable) ? 1 : 0, g_flat_outer_nparams, g_emit.flat_deep_arrival, g_flat_frame_floor, _c2g, _stfj, g_emit.flat_stmt_frame, (void*)g_emit_cfg, (void*)entry); } }   /* STMT-FRAME (s21x-c): classified at the ONE choke point every graph emission passes (the FLATDISP-5b single-writer discipline, same line as FB-STMT above); env-gated SCRIP_STMT_FRAME=1, default OFF until the watermark license (may only add passes, fail-set diffed programmatically).  Guard conjuncts mirror the FB-STMT guard: outer call-regime mains only for slice 1 -- jmp-entry citizens, blobs, generators, resumable and param-bearing graphs keep legacy verbatim (the classifier whitelist already excludes their kinds; the conjuncts make the slice boundary explicit and greppable). */
    /* CARVE-ERAD: SCRIP_NO_GRAPH_CARVE clamp DELETED -- it simulated the carve's absence and the carve is now absent unconditionally. */
    /* s21x-r EXPERIMENT ONLY: clamp the whole-graph carve to the xa_flat >=48 floor to measure whether it is still load-bearing.  NOT a fix -- the 1057 FR/FRQ/FRQB reference sites across 108 templates read slots this carve allocates. */
    g_last_flat_frame_bytes = g_emit_cfg ? g_emit_cfg->jcon_value_region : 0;
    g_last_flat_zstatic = (g_emit_cfg && !g_emit.flat_layout_unknown) ? emit_graph_zstatic(g_emit_cfg) : 0;   /* PS-1b: region-known (arm cleared flat_layout_unknown) AND DEFER/VALUE-free; read only for PAT$ procs downstream, garbage-safe for non-pat emissions because the driver gates the registration on proc_ispat_buf. */
    g_last_flat_fp = (g_emit_cfg && !g_emit.flat_layout_unknown) ? zls_g_fp_total(g_emit_cfg) : 0;   /* PS-3 (s152): interior port-cell term of the DT_P suspension ζ size, same lifecycle as zstatic */
    g_last_flat_uniform = (g_emit_cfg && !g_emit.flat_layout_unknown) ? emit_graph_uniform(g_emit_cfg) : 0;   /* PS-3 (s152): the stride license -- zstatic AND ARBNO-free AND region-known */
    if (out) { emitter_init_text(out, TEXT_MODE_INVOCATION); int rc = codegen_flat_chain_body(entry, prefix); emitter_end(); return rc == 0 ? (bb_box_fn)1 : NULL; }
    bb_buf_t buf = bb_alloc(FLAT_BUF_MAX);
    if (!buf) return NULL;
    g_flat_node_id = 0;
    emitter_init_binary(buf, FLAT_BUF_MAX);
    codegen_flat_chain_body(entry, prefix);
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
void gva_collect_graph(IR_graph_t *g) { if (!g) return; for (int i = 0; i < g->n; i++) { IR_t *nd = g->all[i]; if (!nd) continue; if ((nd->op == IR_VAR || nd->op == IR_ASSIGN || nd->op == IR_VAR_REF) && IR_LIT(nd).sval) (void)gva_collect_var(IR_LIT(nd).sval); } }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void gva_collect_icon_globals(void) {
    extern const char *global_names[]; extern int global_count;
    for (int i = 0; i < global_count; i++) if (global_names[i]) (void)gva_collect_var(global_names[i]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define PL_CATCH_MAX 64
static IR_t *g_pl_catch_nodes[PL_CATCH_MAX];
static int   g_pl_catch_n = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string g_text_acc;
static FILE * g_emit_sink = NULL;
static long   g_emit_text_count = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void emit_set_sink(FILE * out) { g_emit_sink = out; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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

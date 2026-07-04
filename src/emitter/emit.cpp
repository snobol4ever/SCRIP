#ifdef __cplusplus
#include "emit.h"
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
void bb_emit_patch_rel32(bb_label_t *lbl)
{
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
    bb_emit_mode = EMIT_BINARY_WIRED;
    g_platform = BB_PLATFORM_X86; g_medium = BB_MEDIUM_BINARY;
    bb_emit_begin(buf, size);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void emitter_init_text(FILE *out, int mode)
{
    g_is_text = 1; g_emit_text_mode = mode; g_emit_pos = 0;
    bb_emit_mode = EMIT_TEXT;
    g_platform = BB_PLATFORM_X86; g_medium = BB_MEDIUM_TEXT;
    bb_emit_out  = out ? out : stdout;
    emit_io_set_sink(out ? out : stdout);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int  emitter_end(void)        { return g_is_text ? g_emit_pos : bb_emit_end(); }
static void  ef_b1 (uint8_t a)                                   { bb_emit_byte(a); }
static void  ef_b2 (uint8_t a, uint8_t b)                        { bb_emit_byte(a); bb_emit_byte(b); }
static void  ef_b3 (uint8_t a, uint8_t b, uint8_t c)             { bb_emit_byte(a); bb_emit_byte(b); bb_emit_byte(c); }
static void  ef_b4 (uint8_t a, uint8_t b, uint8_t c, uint8_t d)  { bb_emit_byte(a); bb_emit_byte(b); bb_emit_byte(c); bb_emit_byte(d); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void ef_t3c_jmp(const char *mnem, const char *target)
{ fprintf(bb_emit_out, "%s %s\n", mnem ? mnem : "", target ? target : ""); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void emit_label_define_bb(bb_label_t *lbl)
{
    if (g_is_text) {
        char buf[256]; snprintf(buf, sizeof(buf), "%s:", lbl->name);
        fprintf(bb_emit_out, "%s\n", buf);
    } else {
        bb_emit_mode_t s = bb_emit_mode; bb_emit_mode = EMIT_BINARY_WIRED;
        bb_label_define(lbl);
        bb_emit_mode = s;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void emit_jmp_label(bb_label_t *target, jmp_kind_t kind)
{
    static const char    *mn[]    = {"jmp","je","jne","jl","jge","jg"};
    static const uint8_t  ops[6][2] = {{0xE9,0x00},{0x0F,0x84},{0x0F,0x85},{0x0F,0x8C},{0x0F,0x8D},{0x0F,0x8F}};
    int k = (int)kind < 6 ? (int)kind : 0;
    if (g_is_text) { ef_t3c_jmp(mn[k], target->name); g_emit_pos += 6; }
    else { if (k==0) ef_b1(0xE9); else ef_b2(ops[k][0], ops[k][1]); bb_emit_patch_rel32(target); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void emit_aligned_call_rt(const char *sym, void *addr)
{
    if (g_is_text) { fprintf(bb_emit_out, " push rbx\n mov rbx, rsp\n and rsp, -16\n call %s@PLT\n mov rsp, rbx\n pop rbx\n", sym ? sym : ""); }
    else { ef_b1(0x53); ef_b3(0x48,0x89,0xE3); ef_b4(0x48,0x83,0xE4,0xF0); ef_b2(0x48,0xB8); bb_emit_u64((uint64_t)(uintptr_t)addr); ef_b2(0xFF,0xD0); ef_b3(0x48,0x89,0xDC); ef_b1(0x5B); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void bb_label_define(bb_label_t *lbl)
{
    if (!MEDIUM_BINARY) {
        FILE *f = bb_emit_out ? bb_emit_out : stdout;
        char lbuf[256]; snprintf(lbuf, sizeof(lbuf), "%s:", lbl->name);
        fprintf(f, "%s\n", lbuf);
        return;
    }
    if (bb_emit_overflow) return;
    lbl->offset = bb_emit_pos;
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
int g_last_flat_frame_bytes = 0;
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
int g_gvar_flat_chain = 0;
int g_gva_active = 0;
int g_gvar_callarg_live = 0;
int g_emit_frame_caller_dl = -1;
int g_frame_active = 0;
int g_gen_proc_active = 0;
int g_scan_regs_live = 0;
#define FLAT_CHAIN_SET_MAX 512
static IR_t *g_flat_chain_set[FLAT_CHAIN_SET_MAX];
static int   g_flat_chain_set_n = 0;
static bb_label_t *g_limit_gen_beta = NULL;
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
    if ((op >= BINOP_LT && op <= BINOP_NE) || (op >= BINOP_SLT && op <= BINOP_SNE)) return BINOP_CAT_RELOP;
    if (op == BINOP_CONCAT)               return BINOP_CAT_CONCAT;
    if (op == BINOP_ADD || op == BINOP_SUB || op == BINOP_MUL || op == BINOP_DIV || op == BINOP_MOD || op == BINOP_POW) return BINOP_CAT_ARITH;
    if (op == BINOP_CUNION || op == BINOP_CDIFF || op == BINOP_CINTER) return BINOP_CAT_ARITH;
    return BINOP_CAT_ARITH;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int emit_binop_opnd_slot(IR_t *o) {
    if (!o) return -1;
    extern int is_global(const char *);
    if (o->op == IR_VAR && IR_LIT(o).sval && IR_LIT(o).sval[0] != '&' && !is_global(IR_LIT(o).sval)) { int voff = bb_varslot_peek(IR_LIT(o).sval); if (voff >= 0) return voff; }
    int s = bb_slot_get(o); if (s >= 0) return s;
    return (o->tmp >= 0) ? o->tmp : -1;
}
static int binop_operand_real_static(IR_graph_t *g, IR_t *o, int depth);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int binop_operand_real_static(IR_graph_t *g, IR_t *o, int depth) {
    if (!o || depth > 8) return 0;
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
    return binop_operand_real_static(g, bb_child0(nd), 0) || binop_operand_real_static(g, bb_child1(nd), 0);
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
    const char *fn = IR_LIT(nd).sval; int64_t narg = IR_LIT(nd).ival; IR_t *a0 = ir_call_arg(nd, 0);
    if (!(fn && !strcmp(fn, "write") && narg == 1 && a0)) return 0;
    if (bb_slot_get(a0) >= 0) return 1;
    int wintexpr = (a0->op == IR_BINOP || a0->op == IR_LIT_INTEGER || a0->op == IR_TO || a0->op == IR_TO_BY || a0->op == IR_VAR || a0->op == IR_CALL || ir_is_call_kind(a0->op));
    if (wintexpr && (a0->op == IR_BINOP || a0->op == IR_TO || a0->op == IR_TO_BY)) return (a0->op == IR_BINOP && IR_LIT(a0).ival == BINOP_CONCAT) ? 2 : 3;
    if (wintexpr) return 4;
    if (a0->op == IR_LIT_STRING && IR_LIT(a0).sval) return 5;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int bb_call_route_classify(IR_t * nd) {
    const char * fn = g_emit.op_sval ? g_emit.op_sval : ""; int64_t narg = g_emit.op_ival; IR_t * a0 = ir_call_arg(nd, 0); double dv = g_emit.op_dval;
    IR_e k = nd ? nd->op : IR_CALL;
    if (k == IR_CALL_BUILTIN && fn[0] && rt_builtin_is_generator(fn)) return CALL_ROUTE_BYNAME;
    if (k == IR_CALL_PROC_STAGED) return CALL_ROUTE_PROC_STAGED;
    if (k == IR_CALL_BUILTIN && g_emit.op_write_route == 0 && fn[0] && rt_builtin_is_known(fn)) return CALL_ROUTE_FN;
    if (dv == 2.0 && fn[0] && rt_builtin_is_known(fn)) return CALL_ROUTE_BYNAME;
    if (dv == 2.0 && !strcmp(fn, "__rk_bool")) return CALL_ROUTE_RK_BOOL_COND;
    if ((dv == 2.0 || dv == 3.0) && fn[0] && rt_proc_is_registered(fn) && rt_proc_is_generator(fn)) return CALL_ROUTE_PROC_STAGED;
    if ((dv == 2.0 || dv == 3.0) && fn[0] && rt_builtin_is_generator(fn)) return CALL_ROUTE_BYNAME;
    if (dv == 2.0) return CALL_ROUTE_DVAL2_BOMB;
    if (g_gvar_flat_chain && (dv == 2.0 || dv == 3.0) && fn[0] && rt_proc_is_registered(fn)) return CALL_ROUTE_GVAR_USERPROC;
    if (fn[0] && rt_proc_is_registered(fn)) return CALL_ROUTE_PROC_STAGED;
    if (g_gvar_flat_chain && dv == 3.0 && fn[0] && !rt_proc_is_registered(fn)) return CALL_ROUTE_BYNAME;
    if (g_gvar_flat_chain && dv == 2.0 && fn[0] && !rt_proc_is_registered(fn) && !rt_builtin_is_known(fn)) return CALL_ROUTE_BYNAME;
    if (!strcmp(fn, "__rk_bool") && dv == 0.0 && narg == 1 && a0 && bb_slot_get(a0) >= 0) return CALL_ROUTE_RK_BOOL_SLOT;
    switch (g_emit.op_write_route) {
    case 1: return CALL_ROUTE_WRITE_SLOT; case 2: case 3: return CALL_ROUTE_WRITE_BINOP;
    case 4: return CALL_ROUTE_WRITE_LEGACY; case 5: return CALL_ROUTE_WRITE_EMPTY; default: break; }
    if (fn[0] && rt_builtin_is_known(fn)) return CALL_ROUTE_FN;
    return CALL_ROUTE_FATAL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void walk_bb_flat(IR_t *nd, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) { fprintf(stderr, "GROUND ZERO: %s not implemented (Icon-only reset)\n", "walk_bb_flat"); abort(); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int walk_bb_node(IR_t * nd, FILE * out) {
    extern void bb_prepare_capture_arbno(IR_t *nd, int imm);
    extern void bb_prepare(IR_t *nd);
    extern int  bb_slot_get(IR_t *nd);
    if (!nd) return 1;
    g_emit.node = nd;
    emit_io_set_sink(out);
    g_emit.sid  = 0;
    g_emit.nid  = bb_node_id(nd);
    g_emit.x86_uid = g_flat_node_id++;
    g_emit.op_sval = (nd->op == IR_VAR || nd->op == IR_VAR_REF || nd->op == IR_ASSIGN || nd->op == IR_LIT_STRING || nd->op == IR_LIT_CHARSET
                       || nd->op == IR_KEYWORD_ICON || nd->op == IR_KEYWORD_SNOBOL4 || nd->op == IR_KEYWORD_ASSIGN || nd->op == IR_FIELD_GET || nd->op == IR_FIELD_VAR || nd->op == IR_SUBSCRIPT || nd->op == IR_ITERATE
                       || nd->op == IR_MATCH_ASSIGN_COND
                       || nd->op == IR_NULLTEST_VAR || nd->op == IR_PROC_GEN || nd->op == IR_PROC_VALUE || ir_norm_call_kind(nd->op) == IR_CALL)
                    ? IR_LIT(nd).sval : (const char *)0;
    { extern int g_gva_active; extern int gva_index_of(const char *); int nm_op = (nd->op == IR_VAR || nd->op == IR_VAR_REF || nd->op == IR_ASSIGN);
      g_emit.op_gva_k = (g_gva_active && nm_op && IR_LIT(nd).sval) ? gva_index_of(IR_LIT(nd).sval) : -1; }
    { extern int g_proc_direct_active; extern int proc_slot_of(const char *); extern int proc_direct_eligible(const char *); int nm_op = (ir_norm_call_kind(nd->op) == IR_CALL);
      g_emit.op_proc_k = (g_proc_direct_active && nm_op && IR_LIT(nd).sval && proc_direct_eligible(IR_LIT(nd).sval)) ? proc_slot_of(IR_LIT(nd).sval) : -1; }
    g_emit.op_stno = (int32_t)IR_LIT(nd).ival;
    g_emit.op_ival = (ir_norm_call_kind(nd->op) == IR_CALL || nd->op == IR_PROC_GEN) ? (int64_t)nd->n_operands : IR_LIT(nd).ival;
    g_emit.op_node_kind = (int)nd->op;
    g_emit.op_dval = IR_LIT(nd).dval;
    g_emit.op_counter = 0;
    IR_t *op_a = (nd->n_operands > 0) ? nd->operands[0] : (IR_t *)0;
    g_emit.op_a_sval = op_a ? IR_LIT(op_a).sval : (const char *)0;
    g_emit.op_a_node_kind = op_a ? (int)ir_norm_call_kind(op_a->op) : -1;
    g_emit.op_a_slot = (op_a != (IR_t *)0) ? bb_slot_get(op_a) : -1;
    g_emit.op_a_counter = 0;
    g_emit.op_a_ival_sg = op_a ? IR_LIT(op_a).ival : 0;
    g_emit.op_a_dval = op_a ? IR_LIT(op_a).dval : 0;
    { extern int arith_emits_descr(IR_t *nd); g_emit.op_a_descr = arith_emits_descr(op_a) ? 1 : 0; }
    switch (nd->op) {
    case IR_LIT_INTEGER:
    case IR_LIT_STRING:
    case IR_LIT_CHARSET:
    case IR_LIT_REAL:               bb_emit_x86(bb_lit_scalar());         return 0;
    case IR_KEYWORD_ICON:         bb_emit_x86(bb_keyword_icon());       return 0;
    case IR_KEYWORD_SNOBOL4:      bb_emit_x86(bb_keyword_snobol4());    return 0;
    case IR_KEYWORD_ASSIGN:       bb_emit_x86(bb_keyword_assign());     return 0;
    case IR_VAR:                  { extern int is_global(const char *);
        if (IR_LIT(nd).sval && IR_LIT(nd).sval[0] == '&') bb_emit_x86(bb_keyword_icon());
        else if (IR_LIT(nd).sval && is_global(IR_LIT(nd).sval)) bb_emit_x86(bb_var_global());
        else bb_emit_x86(bb_var()); } return 0;
    case IR_VAR_REF:              bb_emit_x86(bb_var_ref());        return 0;
    case IR_ASSIGN: {
        extern int is_global(const char *);
        if (IR_LIT(nd).sval && is_global(IR_LIT(nd).sval)) { bb_emit_x86(bb_assign_global()); return 0; }
        if (IR_LIT(nd).sval) { bb_emit_x86(bb_assign_local()); return 0; }
        fprintf(out, "# [walk_bb_node: kind=%d unhandled]\n", (int)nd->op); return 1;
    }
    case IR_BINOP_TEST:          bb_emit_x86(bb_binop_relop());       return 0;
    case IR_BINOP:
        switch (g_emit.op_binop_kind) {
        case BINOP_CAT_RELOP:  bb_emit_x86(bb_binop_relop());       return 0;
        case BINOP_CAT_CONCAT: bb_emit_x86(bb_binop_concat_slot()); return 0;
        case BINOP_CAT_ARITH:
        default:              bb_emit_x86(bb_binop_arith());       return 0;
        }
    case IR_SUCCEED:              bb_emit_x86(bb_succeed());        return 0;
    case IR_SUSPEND:              bb_emit_x86(bb_suspend());        return 0;
    case IR_TO:                   { bb_prepare(nd); bb_emit_x86(bb_to()); } return 0;
    case IR_MATCH_LEN:            { bb_prepare(nd); bb_emit_x86(bb_match_len()); } return 0;   /* SN4-PAT-1 */
    case IR_MATCH_HEAD:           { bb_emit_x86(bb_match_head()); } return 0;                  /* SN4-PAT-2 */
    case IR_MATCH_ASSIGN_COND:    { bb_emit_x86(bb_match_capture()); } return 0;               /* SN4-PAT-2 */
    case IR_TO_BY:                { bb_prepare(nd); bb_emit_x86(bb_to_by()); } return 0;
    case IR_MAKE_LIST:            bb_emit_x86(bb_make_list());      return 0;
    case IR_CONJUNCTION:                 bb_emit_x86(bb_conjunction());           return 0;
    case IR_GOTO:                 bb_emit_x86(bb_goto());           return 0;
    case IR_SUBSCRIPT:            bb_emit_x86(nd->n_operands == 2 ? bb_subscript() : bb_section()); return 0;
    case IR_DEREF:                bb_emit_x86(bb_deref());          return 0;
    case IR_RANDOM:               bb_emit_x86(bb_random());         return 0;
    case IR_ASSIGN_VAR:           bb_emit_x86(bb_assign_var());     return 0;
    case IR_REV_ASSIGN:                bb_emit_x86(bb_rasgn());          return 0;
    case IR_REV_ASSIGN_VAR:            bb_emit_x86(bb_rev_assign_var()); return 0;
    case IR_SWAP:                 bb_emit_x86(bb_swap());           return 0;
    case IR_SWAP_VAR:             bb_emit_x86(bb_swap_var());       return 0;
    case IR_PROC_VALUE:           bb_emit_x86(bb_proc_value());     return 0;
    case IR_CALL_VALUE:           bb_emit_x86(bb_call_value());     return 0;
    case IR_LIMIT:                bb_emit_x86(bb_limit());          return 0;
    case IR_REPALT:                return 0;
    case IR_ITERATE:              bb_emit_x86(bb_iterate(nd));      return 0;
    case IR_SCAN_ENTER:           { g_emit.op_sb = 1; g_emit.op_sa = g_emit.op_a_slot; bb_emit_x86(bb_gen_scan()); } return 0;
    case IR_INITIAL:           bb_emit_x86(bb_enter_init());     return 0;
    case IR_CREATE:                bb_emit_x86(bb_create());        return 0;
    case IR_ACTIVATE:              bb_emit_x86(bb_activate());      return 0;
    case IR_CORET:                 bb_emit_x86(bb_coret());         return 0;
    case IR_COFAIL:                bb_emit_x86(bb_cofail());        return 0;
    case IR_MOVE_LABEL:            bb_emit_x86(bb_move_label());    return 0;
    case IR_INDIRECT_GOTO: case IR_DISJUNCTION: bb_emit_x86(bb_indirect_goto()); return 0;
    case IR_SCAN:                 { g_emit.op_sb = 0; bb_emit_x86(bb_gen_scan()); }   return 0;
    case IR_SCAN_TAB:             bb_emit_x86(bb_scan_tab());    return 0;
    case IR_SCAN_MOVE:            bb_emit_x86(bb_scan_move());   return 0;
    case IR_SCAN_UPTO:            bb_emit_x86(bb_scan_upto());   return 0;
    case IR_SCAN_ANY:             bb_emit_x86(bb_scan_any());    return 0;
    case IR_SCAN_MANY:            bb_emit_x86(bb_scan_many());   return 0;
    case IR_SCAN_FIND:            bb_emit_x86(bb_scan_find());   return 0;
    case IR_SCAN_MATCH:           bb_emit_x86(bb_scan_match());  return 0;
    case IR_SCAN_POS:             bb_emit_x86(bb_scan_pos());    return 0;
    case IR_SCAN_BAL:             bb_emit_x86(bb_scan_bal());    return 0;
    case IR_RETURN: {
        IR_t *rv = (nd->n_operands > 0 && nd->operands[0]) ? nd->operands[0] : (IR_t *)0;
        g_emit.op_sa = rv ? bb_slot_get(rv) : -1; g_emit.op_dval = IR_LIT(nd).dval; bb_emit_x86(bb_return()); return 0; }
    case IR_CALL_PROC_STAGED: case IR_CALL_BUILTIN:
    case IR_PROC_GEN:
    case IR_CALL: {
        bb_emit_x86(bb_call(nd));
        return 0;
    }
    case IR_FAIL:            bb_emit_x86(bb_fail());                            return 0;
    case IR_UNOP:
    case IR_UNOP_TEST: case IR_NULLTEST_VAR: bb_emit_x86(bb_unop());   return 0;
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
extern int           gva_index_of(const char *name);
extern int           g_gva_active;
extern IR_graph_t *  g_emit_cfg;
#define DRIVE_FILL(nd,s,f,b) do { \
    bb_fill_alpha(nd); \
    g_emit.lbl_γ=(s)->name; g_emit.lbl_ω=(f)->name; g_emit.lbl_β=(b)->name; \
    g_emit.lbl_γ_p=(s); g_emit.lbl_ω_p=(f); g_emit.lbl_β_p=(b); \
    walk_bb_node((nd), emit_outf()); } while(0)
#define DRIVE_PAIR_RESET()      do { g_emit.xa_bb_emit_pair_n = 0; } while(0)
#define DRIVE_PAIR_JMP(tgt)     do { int _i=g_emit.xa_bb_emit_pair_n++; g_emit.xa_bb_emit_pair_define[_i]=NULL; g_emit.xa_bb_emit_pair_jmp[_i]=(tgt); } while(0)
#define DRIVE_PAIR_DEF_JMP(l,t) do { int _i=g_emit.xa_bb_emit_pair_n++; g_emit.xa_bb_emit_pair_define[_i]=(l); g_emit.xa_bb_emit_pair_jmp[_i]=(t); } while(0)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int drive_value_slot(IR_t *nd) {
    int e = bb_slot_get(nd);
    if (e >= 0) return e;
    if (nd && nd->tmp >= 0) { bb_slot_register(nd, nd->tmp); return nd->tmp; }
    if (nd && ir_node_produces_value(nd->op)) {
        fprintf(stderr, "FATAL drive_value_slot: IR op=%d is a value-producer with no nd->tmp — ir_drive_slot_assign never granted it. "
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
    DRIVE_FILL(nodes[i], node_γ, node_ω, betas[i]);
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
void emit_drive(IR_t *nd, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    if (!nd) { drive_unowned(nd); return; }
    switch (nd->op) {
    case IR_LIT_STRING: case IR_LIT_INTEGER: case IR_LIT_REAL:
        g_emit.op_off = drive_value_slot(nd); DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_LIT_CHARSET:
        g_emit.op_sval = IR_LIT(nd).sval; g_emit.op_off = drive_value_slot(nd); DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_KEYWORD_ICON:
    case IR_KEYWORD_SNOBOL4:
        g_emit.op_sval = IR_LIT(nd).sval; g_emit.op_off = drive_value_slot(nd); DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_KEYWORD_ASSIGN:
        g_emit.op_sval = IR_LIT(nd).sval; g_emit.op_off = drive_value_slot(nd); DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_VAR: {
        const char *vn = IR_LIT(nd).sval;
        if (vn && vn[0] == '&') { g_emit.op_sval = vn; g_emit.op_sa = -1; g_emit.op_off = drive_value_slot(nd); }
        else if (vn && is_global(vn)) { g_emit.op_sa = -1; g_emit.op_off = drive_value_slot(nd); g_emit.op_sval = vn; g_emit.op_gva_k = g_gva_active ? gva_index_of(vn) : -1; }
        else if (vn) { int voff = bb_varslot_peek(vn); g_emit.op_sa = voff; g_emit.op_off = (voff >= 0) ? drive_value_slot(nd) : -1; }
        else { g_emit.op_sa = -1; g_emit.op_off = -1; }
        DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_VAR_REF: {
        const char *vn = IR_LIT(nd).sval;
        if (vn && is_global(vn)) { g_emit.op_sa = -1; g_emit.op_off = drive_value_slot(nd); g_emit.op_sval = vn; g_emit.op_gva_k = g_gva_active ? gva_index_of(vn) : -1; }
        else if (vn) { int voff = bb_varslot_peek(vn); g_emit.op_sa = voff; g_emit.op_off = drive_value_slot(nd); }
        else { g_emit.op_sa = -1; g_emit.op_off = -1; }
        DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_BINOP: case IR_BINOP_TEST: {
        g_emit.op_relop_descr = 0; g_emit.op_num_real = 0; g_emit.op_arith_descr = 0; g_emit.op_gva_k1 = -1; g_emit.op_gva_k2 = -1;
        int sa = -1, sb = -1;
        if (binop_is_num_real(g_emit_cfg, nd)) { int ra = bb_slot_get(bb_child0(nd)), rb = bb_slot_get(bb_child1(nd)); if (ra >= 0 && rb >= 0) { sa = ra; sb = rb; g_emit.op_num_real = 1; } }
        if (!g_emit.op_num_real) { sa = emit_binop_opnd_slot(bb_child0(nd)); sb = emit_binop_opnd_slot(bb_child1(nd)); }
        if (sa < 0 || sb < 0) { drive_unowned(nd); break; }
        g_emit.op_sa = sa; g_emit.op_sb = sb; g_emit.op_off = drive_value_slot(nd); g_emit.op_binop_kind = (int)binop_slot_kind(nd);
        DRIVE_PAIR_RESET(); DRIVE_PAIR_DEF_JMP(lbl_β, lbl_ω); DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_UNOP: case IR_UNOP_TEST: case IR_NULLTEST_VAR: {
        int sa = emit_binop_opnd_slot(bb_child0(nd));
        if (sa < 0) { drive_unowned(nd); break; }
        g_emit.op_sa = sa; g_emit.op_off = drive_value_slot(nd); DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_FIELD_GET: case IR_FIELD_VAR: {
        IR_t * obj = bb_child0(nd);
        int sa = obj ? emit_binop_opnd_slot(obj) : -1;
        if (sa < 0) { drive_unowned(nd); break; }
        g_emit.op_a_slot = sa; g_emit.op_sval = IR_LIT(nd).sval; g_emit.op_off = drive_value_slot(nd);
        DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_ASSIGN: {
        const char *vn = IR_LIT(nd).sval;
        if (!vn) {
            fprintf(stderr, "FATAL emit_drive IR_ASSIGN guard: nameless %d-operand assign (assign-through-lvalue-producer: !x/?x element-variable or "
                            "s[i:j] section) — LOWER's TT_ASSIGN terminal arm minted a placeholder; not a missing template. ASSIGN-LV rung, GOAL-IR-IMMUTABLE-EMIT.md.\n", nd->n_operands);
            abort();
        }
        if (is_global(vn)) {
            g_emit.op_sb = -1; g_emit.op_sval = vn; g_emit.op_gva_k = g_gva_active ? gva_index_of(vn) : -1;
            g_emit.op_off = drive_value_slot(nd); DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
        }
        { int voff = bb_varslot_peek(vn);
          if (voff < 0) { fprintf(stderr, "[TE-4] IR_ASSIGN local '%s' has no LOWER-granted varslot — grant it in ir_drive_slot_assign (scrip_ir.c), never allocate in the emitter\n", vn); abort(); }
          g_emit.op_sb = voff; }
        g_emit.op_off = drive_value_slot(nd); DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_CALL: case IR_CALL_BUILTIN: case IR_CALL_PROC_STAGED: case IR_PROC_GEN: {
        int na = nd->n_operands; drive_arg_slots_reserve(na);
        for (int i = 0; i < na; i++) { IR_t * a = ir_call_arg(nd, i); g_emit.op_arg_slot[i] = (a && a->tmp >= 0) ? a->tmp : -1; }
        g_emit.op_arg_slot_n = na; g_emit.op_write_route = bb_call_write_route(nd);
        DRIVE_PAIR_RESET(); DRIVE_PAIR_DEF_JMP(lbl_β, lbl_ω); DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_MATCH_HEAD: {
        IR_t *subj = nd->n_operands > 0 ? nd->operands[0] : (IR_t *)0;
        g_emit.op_sa = subj ? drive_value_slot(subj) : -1;
        g_emit.op_off = drive_value_slot(nd);
        DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_MATCH_ASSIGN_COND: {
        IR_t *box = nd->n_operands > 1 ? nd->operands[1] : (IR_t *)0;
        g_emit.op_off = box ? drive_value_slot(box) : -1; g_emit.op_phase = 1;
        DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_MATCH_LEN: {
        g_emit.op_sa = -1; g_emit.op_off = -1;
        DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_TO: {
        if (!bb_child0(nd) || !bb_child1(nd)) { drive_unowned(nd); break; }
        g_emit.op_sa = drive_value_slot(bb_child0(nd)); g_emit.op_sb = drive_value_slot(bb_child1(nd));
        g_emit.op_sc = -1;
        g_emit.op_num_real = (IR_LIT(nd).sval && strcmp(IR_LIT(nd).sval, "ar") == 0) ? 1 : 0;
        g_emit.op_off = drive_value_slot(nd);
        DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_TO_BY: {
        if (!bb_child0(nd) || !bb_child1(nd) || nd->n_operands < 3 || !nd->operands[2]) { drive_unowned(nd); break; }
        g_emit.op_sa = drive_value_slot(bb_child0(nd)); g_emit.op_sb = drive_value_slot(bb_child1(nd));
        g_emit.op_sc = drive_value_slot(nd->operands[2]);
        g_emit.op_num_real = 0;
        g_emit.op_off = drive_value_slot(nd);
        DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_MAKE_LIST: {
        int na = nd->n_operands; drive_arg_slots_reserve(na);
        for (int i = 0; i < na; i++) { IR_t * a = nd->operands[i]; g_emit.op_arg_slot[i] = a ? drive_value_slot(a) : -1; }
        g_emit.op_arg_slot_n = na;
        g_emit.op_off = drive_value_slot(nd);
        DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
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
        DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_REV_ASSIGN_VAR: {
        IR_t * v = nd->n_operands > 0 ? nd->operands[0] : NULL;
        IR_t * r = nd->n_operands > 1 ? nd->operands[1] : NULL;
        int sa = v ? drive_value_slot(v) : -1; int sb = r ? drive_value_slot(r) : -1;
        if (sa < 0 || sb < 0) { drive_unowned(nd); break; }
        g_emit.op_sa  = sb;
        g_emit.op_off = drive_value_slot(nd);
        g_emit.op_sc  = g_emit.op_off + 16;
        DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_GOTO:
        DRIVE_PAIR_RESET(); DRIVE_PAIR_JMP(lbl_γ); DRIVE_PAIR_DEF_JMP(lbl_β, lbl_ω); DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_CONJUNCTION:
        if (nd->n_operands > 0 && nd->operands[0] && bb_slot_get(nd) < 0) { int voff = bb_slot_get(nd->operands[0]); if (voff >= 0) bb_slot_register(nd, voff); }
        if (nd->tmp >= 0 && nd->n_operands > 0 && nd->operands[0]) {
            int src = bb_slot_get(nd->operands[0]);
            int dst = nd->tmp;
            if (src >= 0 && dst >= 0 && src != dst) {
                g_emit.op_sa = src; g_emit.op_off = dst;
            } else {
                g_emit.op_sa = -1; g_emit.op_off = -1;
            }
        } else {
            g_emit.op_sa = -1; g_emit.op_off = -1;
        }
        DRIVE_PAIR_RESET(); DRIVE_PAIR_JMP(lbl_γ); DRIVE_PAIR_DEF_JMP(lbl_β, lbl_ω); DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_SUCCEED:
        DRIVE_PAIR_RESET(); DRIVE_PAIR_JMP(lbl_γ); DRIVE_PAIR_DEF_JMP(lbl_β, lbl_ω); DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_SUSPEND: {
        IR_t * ev = bb_child0(nd); int sa = ev ? bb_slot_get(ev) : -1;
        if (sa < 0) { drive_unowned(nd); break; }
        g_emit.op_sa = sa; g_emit.lbl_t0 = g_suspend_dobody_beta ? g_suspend_dobody_beta->name : NULL; g_emit.lbl_t0_p = g_suspend_dobody_beta;
        g_emit.lbl_t1_p = lbl_β;
        g_emit.op_sb    = g_suspend_resume_slot;
        DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_SUBSCRIPT: {
        if (nd->n_operands == 2) {
            IR_t * vb = nd->operands[0]; IR_t * vi = nd->operands[1];
            int va = vb ? drive_value_slot(vb) : -1; int vs = vi ? drive_value_slot(vi) : -1;
            if (va < 0 || vs < 0) { drive_unowned(nd); break; }
            g_emit.op_a_slot = va; g_emit.op_sa = vs; g_emit.op_off = drive_value_slot(nd);
            DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
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
        DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_RANDOM: {
        IR_t * v = nd->n_operands > 0 ? nd->operands[0] : NULL;
        int sa = v ? drive_value_slot(v) : -1;
        if (sa < 0) { drive_unowned(nd); break; }
        g_emit.op_a_slot = sa; g_emit.op_off = drive_value_slot(nd);
        DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_DEREF: {
        IR_t * v = nd->n_operands > 0 ? nd->operands[0] : NULL;
        int sa = v ? drive_value_slot(v) : -1;
        if (sa < 0) { drive_unowned(nd); break; }
        g_emit.op_a_slot = sa; g_emit.op_off = drive_value_slot(nd);
        DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_ASSIGN_VAR: {
        IR_t * v = nd->n_operands > 0 ? nd->operands[0] : NULL;
        IR_t * r = nd->n_operands > 1 ? nd->operands[1] : NULL;
        int sa = v ? drive_value_slot(v) : -1; int sb = r ? drive_value_slot(r) : -1;
        if (sa < 0 || sb < 0) { drive_unowned(nd); break; }
        g_emit.op_a_slot = sa; g_emit.op_sa = sb; g_emit.op_off = drive_value_slot(nd);
        DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_SWAP: {
        IR_t * xv = nd->n_operands > 0 ? nd->operands[0] : NULL;
        IR_t * yv = nd->n_operands > 1 ? nd->operands[1] : NULL;
        int sa = xv ? bb_varslot_peek(IR_LIT(xv).sval) : -1;
        int sb = yv ? bb_varslot_peek(IR_LIT(yv).sval) : -1;
        if (sa < 0 || sb < 0) { drive_unowned(nd); break; }
        g_emit.op_sa = sa; g_emit.op_sb = sb; g_emit.op_off = drive_value_slot(nd);
        DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_PROC_VALUE: {
        g_emit.op_off = drive_value_slot(nd);
        DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
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
        DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_SWAP_VAR: {
        IR_t * xv = nd->n_operands > 0 ? nd->operands[0] : NULL;
        IR_t * yv = nd->n_operands > 1 ? nd->operands[1] : NULL;
        int sa = xv ? drive_value_slot(xv) : -1; int sb = yv ? drive_value_slot(yv) : -1;
        if (sa < 0 || sb < 0) { drive_unowned(nd); break; }
        g_emit.op_a_slot = sa; g_emit.op_sa = sb; g_emit.op_off = drive_value_slot(nd);
        DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
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
        DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_CREATE: {
        g_emit.op_off = drive_value_slot(nd);
        g_emit.lbl_t0   = g_create_body_entry ? g_create_body_entry->name : NULL;
        g_emit.lbl_t0_p = g_create_body_entry;
        DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_ACTIVATE: {
        IR_t * ce = nd->n_operands > 0 ? nd->operands[0] : NULL;
        IR_t * xv = nd->n_operands > 1 ? nd->operands[1] : NULL;
        int sa = ce ? bb_slot_get(ce) : -1;
        if (sa < 0) { drive_unowned(nd); break; }
        g_emit.op_sa = sa;
        g_emit.op_sb = xv ? bb_slot_get(xv) : -1;
        g_emit.op_off = drive_value_slot(nd);
        DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_CORET: {
        IR_t * pv = nd->n_operands > 0 ? nd->operands[0] : NULL;
        int sa = pv ? bb_slot_get(pv) : -1;
        if (sa < 0) { drive_unowned(nd); break; }
        g_emit.op_sa = sa;
        DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_COFAIL: {
        DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_MOVE_LABEL: {
        IR_t * ig = nd->n_operands > 1 ? nd->operands[1] : NULL;
        IR_t * av = nd->n_operands > 2 ? nd->operands[2] : NULL;
        int off = ig ? drive_value_slot(ig) : -1;
        g_emit.op_off = off;
        g_emit.op_sa = av ? bb_slot_get(av) : -1;
        g_emit.lbl_t0 = g_move_label_tgt ? g_move_label_tgt->name : NULL;
        g_emit.lbl_t0_p = g_move_label_tgt;
        DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_INDIRECT_GOTO: case IR_DISJUNCTION: {
        int off = drive_value_slot(nd);
        g_emit.op_off = off;
        DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_ITERATE: {
        IR_t * obj = nd->n_operands > 0 ? nd->operands[0] : NULL;
        int sa = obj ? bb_slot_get(obj) : -1;
        if (sa < 0) { drive_unowned(nd); break; }
        g_emit.op_sa = sa;
        g_emit.op_off = drive_value_slot(nd);
        g_emit.op_sb  = g_emit.op_off + 16;
        DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_INITIAL: {
        g_emit.op_off = nd->tmp;
        if (g_emit.op_off < 0) { drive_unowned(nd); break; }
        DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_SCAN_ENTER: {
        IR_t * subj = nd->n_operands > 0 ? nd->operands[0] : NULL;
        int sa = subj ? bb_slot_get(subj) : -1;
        if (sa < 0) { drive_unowned(nd); break; }
        g_emit.op_sa = sa; g_emit.op_sb = 1;
        g_emit.op_off = drive_value_slot(nd);
        { extern int g_scan_regs_live; g_scan_regs_live++; }
        DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_SCAN: {
        g_emit.op_sb  = 0;
        { extern int g_scan_regs_live; if (g_scan_regs_live > 0) g_scan_regs_live--; }
        IR_t * enter_nd = nd->n_operands > 0 ? nd->operands[0] : NULL;
        g_emit.op_off = (enter_nd && enter_nd->tmp >= 0) ? enter_nd->tmp : -1;
        if (g_emit.op_off < 0) { drive_unowned(nd); break; }
        DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_SCAN_TAB: {
        IR_t * a0 = nd->n_operands > 0 ? nd->operands[0] : NULL;
        g_emit.op_off = drive_value_slot(nd);
        if (!a0) { drive_unowned(nd); break; }
        if (a0->op == IR_LIT_INTEGER) { g_emit.op_sb = (int)IR_LIT(a0).ival; g_emit.op_sa = -1; }
        else { int sl = bb_slot_get(a0); if (sl < 0) { drive_unowned(nd); break; } g_emit.op_sa = sl; g_emit.op_sb = 0; }
        DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_SCAN_MOVE: {
        IR_t * a0 = nd->n_operands > 0 ? nd->operands[0] : NULL;
        g_emit.op_off = drive_value_slot(nd);
        if (!a0) { drive_unowned(nd); break; }
        if (a0->op == IR_LIT_INTEGER) { g_emit.op_sb = (int)IR_LIT(a0).ival; g_emit.op_sa = -1; }
        else { int sl = bb_slot_get(a0); if (sl < 0) { drive_unowned(nd); break; } g_emit.op_sa = sl; g_emit.op_sb = 0; }
        DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_SCAN_POS: {
        IR_t * a0 = nd->n_operands > 0 ? nd->operands[0] : NULL;
        g_emit.op_off = drive_value_slot(nd);
        if (!a0) { drive_unowned(nd); break; }
        if (a0->op == IR_LIT_INTEGER) { g_emit.op_sb = (int)IR_LIT(a0).ival; g_emit.op_sa = -1; }
        else { int sl = bb_slot_get(a0); if (sl < 0) { drive_unowned(nd); break; } g_emit.op_sa = sl; g_emit.op_sb = 0; }
        DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_SCAN_UPTO: {
        IR_t * a0 = nd->n_operands > 0 ? nd->operands[0] : NULL;
        g_emit.op_off = drive_value_slot(nd);
        if (a0 && (a0->op == IR_LIT_STRING || a0->op == IR_LIT_CHARSET) && IR_LIT(a0).sval) {
            g_emit.op_name1 = IR_LIT(a0).sval; g_emit.op_sa = -1;
        } else if (a0) {
            int sl = bb_slot_get(a0); if (sl < 0) { drive_unowned(nd); break; } g_emit.op_sa = sl; g_emit.op_name1 = NULL;
        } else { drive_unowned(nd); break; }
        DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_SCAN_ANY: {
        IR_t * a0 = nd->n_operands > 0 ? nd->operands[0] : NULL;
        g_emit.op_off = drive_value_slot(nd);
        if (a0 && (a0->op == IR_LIT_STRING || a0->op == IR_LIT_CHARSET) && IR_LIT(a0).sval) {
            g_emit.op_name1 = IR_LIT(a0).sval; g_emit.op_sa = -1;
        } else if (a0) {
            int sl = bb_slot_get(a0); if (sl < 0) { drive_unowned(nd); break; } g_emit.op_sa = sl; g_emit.op_name1 = NULL;
        } else { drive_unowned(nd); break; }
        DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_SCAN_MANY: {
        IR_t * a0 = nd->n_operands > 0 ? nd->operands[0] : NULL;
        g_emit.op_off = drive_value_slot(nd);
        if (a0 && (a0->op == IR_LIT_STRING || a0->op == IR_LIT_CHARSET) && IR_LIT(a0).sval) {
            g_emit.op_name1 = IR_LIT(a0).sval; g_emit.op_sa = -1;
        } else if (a0) {
            int sl = bb_slot_get(a0); g_emit.op_sa = sl; g_emit.op_name1 = NULL;
        } else { g_emit.op_name1 = NULL; g_emit.op_sa = -1; }
        DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_SCAN_FIND: {
        IR_t * a0 = nd->n_operands > 0 ? nd->operands[0] : NULL;
        g_emit.op_off = drive_value_slot(nd);
        if (a0 && a0->op == IR_LIT_STRING && IR_LIT(a0).sval) {
            g_emit.op_name1 = IR_LIT(a0).sval; g_emit.op_sa = -1;
        } else if (a0) {
            int sl = bb_slot_get(a0); if (sl < 0) { drive_unowned(nd); break; } g_emit.op_sa = sl; g_emit.op_name1 = NULL;
        } else { drive_unowned(nd); break; }
        DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_SCAN_MATCH: {
        IR_t * a0 = nd->n_operands > 0 ? nd->operands[0] : NULL;
        g_emit.op_off = drive_value_slot(nd);
        if (a0 && a0->op == IR_LIT_STRING && IR_LIT(a0).sval) {
            g_emit.op_name1 = IR_LIT(a0).sval; g_emit.op_sa = -1;
        } else if (a0) {
            int sl = bb_slot_get(a0); if (sl < 0) { drive_unowned(nd); break; } g_emit.op_sa = sl; g_emit.op_name1 = NULL;
        } else { drive_unowned(nd); break; }
        DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_SCAN_BAL: {
        IR_t * a0 = nd->n_operands > 0 ? nd->operands[0] : NULL;
        g_emit.op_off = drive_value_slot(nd);
        if (a0 && (a0->op == IR_LIT_STRING || a0->op == IR_LIT_CHARSET) && IR_LIT(a0).sval) {
            g_emit.op_name1 = IR_LIT(a0).sval; g_emit.op_sa = -1;
        } else if (a0) {
            int sl = bb_slot_get(a0); if (sl < 0) { drive_unowned(nd); break; } g_emit.op_sa = sl; g_emit.op_name1 = NULL;
        } else { drive_unowned(nd); break; }
        DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_REPALT: {
        drive_unowned(nd); break;
    }
    case IR_FAIL:
        DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_RETURN:
        DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    default:
        drive_unowned(nd); break;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
    entry = entry;
    queue[qt++] = entry;
    while (qh < qt) {
        IR_t *c = queue[qh++];
        if (!c || c->op == IR_SUCCEED || c->op == IR_FAIL) continue;
        int dup = 0; for (int i = 0; i < n; i++) if (nodes[i] == c) { dup = 1; break; }
        if (dup) continue;
        if (n >= CH_MAX) { fprintf(stderr, "[GZ-7] FATAL chain exceeds CH_MAX\n"); abort(); }
        nodes[n++] = c;
        if (c->γ.node && qt < CH_MAX) queue[qt++] = c->γ.node;
        if ((c->op == IR_BINOP || c->op == IR_BINOP_TEST || c->op == IR_UNOP || c->op == IR_UNOP_TEST || c->op == IR_NULLTEST_VAR) && c->ω.node && qt < CH_MAX) queue[qt++] = c->ω.node;
        if ((c->op == IR_CALL || ir_is_call_kind(c->op) || c->op == IR_PROC_GEN || c->op == IR_ACTIVATE) && c->ω.node && qt < CH_MAX) queue[qt++] = c->ω.node;
        if ((c->op == IR_SUBSCRIPT || c->op == IR_RANDOM || c->op == IR_DEREF || c->op == IR_ASSIGN_VAR || c->op == IR_REV_ASSIGN_VAR || c->op == IR_KEYWORD_ASSIGN || c->op == IR_SCAN_TAB || c->op == IR_SCAN_MOVE || c->op == IR_SCAN_POS || c->op == IR_SCAN_MATCH || c->op == IR_SCAN_ANY
             || c->op == IR_SWAP_VAR || c->op == IR_CALL_VALUE) && c->ω.node && qt < CH_MAX)
            queue[qt++] = c->ω.node;
        if (c->op == IR_SUSPEND && c->n_operands > 1 && c->operands[1] && qt < CH_MAX) queue[qt++] = c->operands[1];
        if (c->op == IR_CREATE && c->n_operands > 0 && c->operands[0] && qt < CH_MAX) queue[qt++] = c->operands[0];
        if (c->op == IR_MOVE_LABEL && c->n_operands > 0 && c->operands[0] && qt < CH_MAX) queue[qt++] = c->operands[0];
        if (c->op == IR_MOVE_LABEL && c->n_operands > 1 && c->operands[1] && qt < CH_MAX) queue[qt++] = c->operands[1];
        if (c->op == IR_REPALT && c->n_operands > 0 && c->operands[0] && qt < CH_MAX) queue[qt++] = c->operands[0];
        if (c->op == IR_REPALT && c->n_operands > 1 && c->operands[1] && qt < CH_MAX) queue[qt++] = c->operands[1];
    }
    for (int i = 0; i < n; i++) if (ir_is_generator_kind(nodes[i]->op) && nodes[i]->ω.node) {
        int present = 0; for (int j = 0; j < n; j++) if (nodes[j] == nodes[i]->ω.node) { present = 1; break; }
        if (!present && qt < CH_MAX) queue[qt++] = nodes[i]->ω.node; }
    while (qh < qt) {
        IR_t *c = queue[qh++];
        if (!c || c->op == IR_SUCCEED || c->op == IR_FAIL) continue;
        int dup = 0; for (int i = 0; i < n; i++) if (nodes[i] == c) { dup = 1; break; }
        if (dup) continue;
        if (n >= CH_MAX) { fprintf(stderr, "[GZ-7] FATAL chain exceeds CH_MAX\n"); abort(); }
        nodes[n++] = c;
        if (c->γ.node && qt < CH_MAX) queue[qt++] = c->γ.node;
        if ((c->op == IR_BINOP || c->op == IR_BINOP_TEST || c->op == IR_UNOP || c->op == IR_UNOP_TEST || c->op == IR_NULLTEST_VAR) && c->ω.node && qt < CH_MAX) queue[qt++] = c->ω.node;
        if ((c->op == IR_CALL || ir_is_call_kind(c->op) || c->op == IR_PROC_GEN || c->op == IR_ACTIVATE) && c->ω.node && qt < CH_MAX) queue[qt++] = c->ω.node;
        if ((c->op == IR_SUBSCRIPT || c->op == IR_RANDOM || c->op == IR_DEREF || c->op == IR_ASSIGN_VAR || c->op == IR_REV_ASSIGN_VAR || c->op == IR_KEYWORD_ASSIGN || c->op == IR_SCAN_TAB || c->op == IR_SCAN_MOVE || c->op == IR_SCAN_POS || c->op == IR_SCAN_MATCH || c->op == IR_SCAN_ANY
             || c->op == IR_SWAP_VAR || c->op == IR_CALL_VALUE) && c->ω.node && qt < CH_MAX)
            queue[qt++] = c->ω.node;
        if (ir_is_generator_kind(c->op) && c->ω.node && qt < CH_MAX) queue[qt++] = c->ω.node;
        if (c->op == IR_SUSPEND && c->n_operands > 1 && c->operands[1] && qt < CH_MAX) queue[qt++] = c->operands[1];
        if (c->op == IR_CREATE && c->n_operands > 0 && c->operands[0] && qt < CH_MAX) queue[qt++] = c->operands[0];
        if (c->op == IR_MOVE_LABEL && c->n_operands > 0 && c->operands[0] && qt < CH_MAX) queue[qt++] = c->operands[0];
        if (c->op == IR_MOVE_LABEL && c->n_operands > 1 && c->operands[1] && qt < CH_MAX) queue[qt++] = c->operands[1];
        if (c->op == IR_REPALT && c->n_operands > 0 && c->operands[0] && qt < CH_MAX) queue[qt++] = c->operands[0];
        if (c->op == IR_REPALT && c->n_operands > 1 && c->operands[1] && qt < CH_MAX) queue[qt++] = c->operands[1];
    }
    bb_label_t **lbls  = (bb_label_t **)alloca(sizeof(bb_label_t *) * n);
    bb_label_t **betas = (bb_label_t **)alloca(sizeof(bb_label_t *) * n);
    bb_label_t **ra_y  = (bb_label_t **)alloca(sizeof(bb_label_t *) * n);
    bb_label_t **ra_t  = (bb_label_t **)alloca(sizeof(bb_label_t *) * n);
    for (int i = 0; i < n && g_flat_chain_set_n < FLAT_CHAIN_SET_MAX; i++) g_flat_chain_set[g_flat_chain_set_n++] = nodes[i];
    g_suspend_resume_slot = -1;
    if (g_gen_proc_active && g_emit_cfg && g_emit_cfg->resume_slot >= 0)
        for (int _si = 0; _si < n; _si++) if (nodes[_si]->op == IR_SUSPEND) { g_suspend_resume_slot = g_emit_cfg->resume_slot; break; }
    int id = g_flat_node_id++;
    for (int i = 0; i < n; i++) {
        lbls[i]  = emit_label_alloc("xchain%d_n%d_α", id, i);
        betas[i] = emit_label_alloc("xchain%d_n%d_β", id, i);
        ra_y[i]  = (nodes[i]->op == IR_REPALT) ? emit_label_alloc("xchain%d_n%d_ry", id, i) : NULL;
        ra_t[i]  = (nodes[i]->op == IR_REPALT) ? emit_label_alloc("xchain%d_n%d_rt", id, i) : NULL;
    }
    if (g_suspend_resume_slot >= 0 && g_gen_proc_active) {
        for (int _si = 0; _si < n; _si++) if (nodes[_si]->op == IR_SUSPEND) {
            if (g_is_text) {
                char _init[256];
                snprintf(_init, sizeof _init,
                    "lea rax, [rip + %s]\nmov qword ptr [r12 + %d], rax\n",
                    betas[_si]->name, g_suspend_resume_slot);
                emit_text_n(_init, strlen(_init));
            } else {
                ef_b3(0x48, 0x8D, 0x05); bb_emit_patch_rel32(betas[_si]);
                ef_b4(0x49, 0x89, 0x84, 0x24); bb_emit_u32((uint32_t)(unsigned)g_suspend_resume_slot);
            }
            break;
        }
    }
    for (int _li = 0; _li < n; _li++) if (nodes[_li]->op == IR_LIMIT) {
        g_emit.op_off = drive_value_slot(nodes[_li]);
        bb_emit_x86(bb_limit_init());
    }
    for (int i = 0; i < n; i++) {
        emit_label_define_bb(lbls[i]);
        bb_label_t *node_γ = &lbl_γ;
        bb_label_t *node_ω = &lbl_ω;
        IR_t *gtgt = nodes[i]->γ.node;
        IR_t *otgt = nodes[i]->ω.node;
        { int _gg = 0; while (gtgt && gtgt->op == IR_GOTO && _gg++ < 128) gtgt = gtgt->γ.node; }
        { int _gg = 0; while (otgt && otgt->op == IR_GOTO && _gg++ < 128) otgt = otgt->γ.node; }
        int gamma_is_beta = (nodes[i]->γ.sz[0] == (char)0xce && (unsigned char)nodes[i]->γ.sz[1] == 0xb2);
        int omega_is_beta = (nodes[i]->ω.sz[0] == (char)0xce && (unsigned char)nodes[i]->ω.sz[1] == 0xb2);
        for (int k = 0; k < n; k++) if (nodes[k] == gtgt) {
            node_γ = gamma_is_beta ? betas[k] : lbls[k];
            break;
        }
        if (nodes[i]->γ.node == NULL || nodes[i]->γ.node->op == IR_SUCCEED) node_γ = &lbl_γ;
        if (nodes[i]->γ.node && nodes[i]->γ.node->op == IR_FAIL) node_γ = &lbl_ω;
        int omega_resolved = 0;
        for (int k = 0; k < n; k++) if (nodes[k] == otgt) { node_ω = omega_is_beta ? betas[k] : lbls[k]; omega_resolved = 1; break; }
        if (!omega_resolved) node_ω = (otgt && otgt->op == IR_SUCCEED) ? &lbl_γ : &lbl_ω;
        for (int r = 0; r < n; r++) if (nodes[r]->op == IR_REPALT && nodes[r]->n_operands > 0 && nodes[r]->operands[0] == nodes[i]) { node_γ = ra_y[r]; node_ω = ra_t[r]; break; }
        if (nodes[i]->op == IR_CONJUNCTION && nodes[i]->n_operands > 0 && nodes[i]->operands[0]) {
            IR_t * op0 = nodes[i]->operands[0];
            if (op0->tmp >= 0 && nodes[i]->tmp < 0) {
                bb_slot_register(nodes[i], op0->tmp);
            }
        }
        g_limit_gen_beta = NULL;
        g_create_body_entry = NULL;
        g_move_label_tgt = NULL;
        g_suspend_dobody_beta = NULL;
        if (nodes[i]->op == IR_SUSPEND && nodes[i]->n_operands > 1 && nodes[i]->operands[1]) {
            IR_t *dobody = nodes[i]->operands[1];
            if (dobody->op == IR_FAIL) { g_suspend_dobody_beta = &lbl_ω; }
            else { for (int k = 0; k < n; k++) if (nodes[k] == dobody) { g_suspend_dobody_beta = lbls[k]; break; } }
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
            flat_drive_repalt(nodes, n, i, lbls, betas, ra_y, ra_t, node_γ, node_ω);
            continue;
        }
        if (nodes[i]->op == IR_LIMIT) {
            IR_t *gen = nodes[i]->n_operands > 0 ? nodes[i]->operands[0] : NULL;
            if (gen) for (int k = 0; k < n; k++) if (nodes[k] == gen) { g_limit_gen_beta = betas[k]; break; }
        }
        emit_drive(nodes[i], node_γ, node_ω, betas[i]);
    }
    emit_label_define_bb(&lbl_β);
    if (g_suspend_resume_slot >= 0 && g_gen_proc_active) {
        if (g_is_text) {
            char _ind_jmp[64];
            snprintf(_ind_jmp, sizeof _ind_jmp, "jmp qword ptr [r12 + %d]\n", g_suspend_resume_slot);
            emit_text_n(_ind_jmp, strlen(_ind_jmp));
        } else {
            ef_b4(0x49, 0xFF, 0xA4, 0x24); bb_emit_u32((uint32_t)(unsigned)g_suspend_resume_slot);
        }
    } else {
        bb_label_t *resume_tgt = &lbl_ω;
        for (int i = 0; i < n; i++) if (nodes[i]->op == IR_SUSPEND) { resume_tgt = betas[i]; break; }
        emit_jmp_label(resume_tgt, JMP_JMP);
    }
    emit_label_define_bb(&lbl_γ);
    xa_dispatch(XA_FLAT_EPILOGUE);
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
    case IR_VAR:   case IR_KEYWORD_ICON: case IR_KEYWORD_SNOBOL4: return 0;
    case IR_BINOP: case IR_TO: return 2;
    case IR_TO_BY: return 3;
    case IR_MAKE_LIST: return n->n_operands;
    case IR_CONJUNCTION: case IR_GOTO: return 0;
    case IR_UNOP: case IR_UNOP_TEST: return 1;
    case IR_ASSIGN: return 1;
    case IR_RETURN: return 1;
    case IR_CALL_PROC_STAGED: case IR_CALL_BUILTIN:
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
    IR_t *chain[512]; int nc = 0;
    IR_t *seen[512]; int ns = 0;
    IR_t *stkv[512]; int sv = 0;
    { int guard = 0; while (entry && (entry->op == IR_SUCCEED || entry->op == IR_FAIL) && entry->γ.node && guard++ < 512) entry = entry->γ.node; }
    entry = entry;
    stkv[sv++] = entry;
    while (sv > 0 && nc < 512) {
        IR_t *c = stkv[--sv];
        if (!c || c->op == IR_SUCCEED || c->op == IR_FAIL) continue;
        int dup = 0; for (int i = 0; i < ns; i++) if (seen[i] == c) { dup = 1; break; }
        if (dup) continue;
        seen[ns++] = c; chain[nc++] = c;
        if ((c->op == IR_BINOP) && c->ω.node && sv < 512) stkv[sv++] = c->ω.node;
        if ((c->op == IR_CALL || ir_is_call_kind(c->op)) && c->ω.node && sv < 512) stkv[sv++] = c->ω.node;
        if ((c->op == IR_SUBSCRIPT || c->op == IR_RANDOM || c->op == IR_DEREF || c->op == IR_ASSIGN_VAR || c->op == IR_REV_ASSIGN_VAR || c->op == IR_KEYWORD_ASSIGN || c->op == IR_SCAN_TAB || c->op == IR_SCAN_MOVE || c->op == IR_SCAN_POS || c->op == IR_SCAN_MATCH || c->op == IR_SCAN_ANY
             || c->op == IR_SWAP_VAR || c->op == IR_CALL_VALUE) && c->ω.node && sv < 512)
            stkv[sv++] = c->ω.node;
        if (c->γ.node && sv < 512) stkv[sv++] = c->γ.node;
    }
    for (int i = 0; i < nc; i++) if (ir_is_generator_kind(chain[i]->op) && chain[i]->ω.node) {
        int present = 0; for (int j = 0; j < ns; j++) if (seen[j] == chain[i]->ω.node) { present = 1; break; }
        if (!present && sv < 512) stkv[sv++] = chain[i]->ω.node; }
    while (sv > 0 && nc < 512) {
        IR_t *c = stkv[--sv];
        if (!c || c->op == IR_SUCCEED || c->op == IR_FAIL) continue;
        int dup = 0; for (int i = 0; i < ns; i++) if (seen[i] == c) { dup = 1; break; }
        if (dup) continue;
        seen[ns++] = c; chain[nc++] = c;
        if ((c->op == IR_BINOP) && c->ω.node && sv < 512) stkv[sv++] = c->ω.node;
        if ((c->op == IR_CALL || ir_is_call_kind(c->op)) && c->ω.node && sv < 512) stkv[sv++] = c->ω.node;
        if ((c->op == IR_SUBSCRIPT || c->op == IR_RANDOM || c->op == IR_DEREF || c->op == IR_ASSIGN_VAR || c->op == IR_REV_ASSIGN_VAR || c->op == IR_KEYWORD_ASSIGN || c->op == IR_SCAN_TAB || c->op == IR_SCAN_MOVE || c->op == IR_SCAN_POS || c->op == IR_SCAN_MATCH || c->op == IR_SCAN_ANY
             || c->op == IR_SWAP_VAR || c->op == IR_CALL_VALUE) && c->ω.node && sv < 512)
            stkv[sv++] = c->ω.node;
        if (ir_is_generator_kind(c->op) && c->ω.node && sv < 512) stkv[sv++] = c->ω.node;
        if (c->γ.node && sv < 512) stkv[sv++] = c->γ.node;
    }
    IR_t *stk[512]; int sp = 0;
    for (int i = 0; i < nc; i++) {
        IR_t *n = chain[i];
        int ar = emit_chain_arity(n);
        if (ar < 0) { sp = 0; continue; }
        else if (ar >= 1 && sp >= ar) { if (n->n_operands < ar) { n->n_operands = 0; for (int k = ar; k >= 1; k--) ir_operand_push(n, stk[sp - k]); } sp -= ar; }
        else if (ar >= 1) { sp = 0; }
        stk[sp++] = n;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
bb_box_fn emit_chain(IR_t *entry, FILE *out, const char *prefix) {
    if (!entry) return NULL;
    emit_chain_operand_refs(entry);
    g_bb_slotmap_n = 0;
    g_flat_chain_set_n = 0;
    g_last_flat_frame_bytes = g_emit_cfg ? g_emit_cfg->jcon_value_region : 0;
    if (out) { emitter_init_text(out, TEXT_MODE_INVOCATION); int rc = codegen_flat_chain_body(entry, prefix); emitter_end(); return rc == 0 ? (bb_box_fn)1 : NULL; }
    bb_buf_t buf = bb_alloc(FLAT_BUF_MAX);
    if (!buf) return NULL;
    g_flat_node_id = 0;
    emitter_init_binary(buf, FLAT_BUF_MAX);
    codegen_flat_chain_body(entry, prefix);
    int nbytes = emitter_end();
    extern int bb_emit_overflow;
    if (bb_emit_overflow || nbytes <= 0 || nbytes > FLAT_BUF_MAX) { bb_free(buf, FLAT_BUF_MAX); return NULL; }
    bb_seal(buf, (size_t)nbytes);
    bb_pool_trim_last(buf, FLAT_BUF_MAX, (size_t)nbytes);
    return (bb_box_fn)buf;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void gva_collect_graph(IR_graph_t *g) { fprintf(stderr, "GROUND ZERO: %s not implemented (Icon-only reset)\n", "gva_collect_graph"); abort(); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void gva_collect_icon_globals(void) {
    extern const char *global_names[]; extern int global_count;
    for (int i = 0; i < global_count; i++) if (global_names[i]) (void)gva_collect_var(global_names[i]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define PL_CATCH_MAX 64
static IR_t *g_pl_catch_nodes[PL_CATCH_MAX];
static int   g_pl_catch_n = 0;
#ifdef __cplusplus
}
#endif

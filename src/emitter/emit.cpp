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
extern const char * Σ;
extern int          Σlen;

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
int g_gva_active = 0;
int g_gvar_callarg_live = 0;
int g_emit_frame_caller_dl = -1;
int g_frame_active = 0;
int g_gen_proc_active = 0;
int g_resumable_callable_active = 0;
int g_scan_regs_live = 0;
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
extern "C" int zls_arbno_geom(const IR_t *, int *, int *);
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
                       || nd->op == IR_KEYWORD_ICON || nd->op == IR_KEYWORD_ICON_GEN || nd->op == IR_KEYWORD_SNOBOL4 || nd->op == IR_KEYWORD_ASSIGN || nd->op == IR_REV_SWAP || nd->op == IR_FIELD_GET || nd->op == IR_FIELD_VAR || nd->op == IR_SUBSCRIPT || nd->op == IR_ITERATE
                       || nd->op == IR_MATCH_ASSIGN_COND || nd->op == IR_MATCH_ASSIGN_SAVE || nd->op == IR_MATCH_ASSIGN_IMM || nd->op == IR_MATCH_LIT || nd->op == IR_MATCH_ANY || nd->op == IR_MATCH_NOTANY || nd->op == IR_MATCH_SPAN
                       || nd->op == IR_MATCH_BREAK || nd->op == IR_MATCH_BREAKX || nd->op == IR_MATCH_DEFER || nd->op == IR_MATCH_ATP || nd->op == IR_MATCH_REPLACE || nd->op == IR_GOTO_DEFERRED
                       || nd->op == IR_NULLTEST_VAR || nd->op == IR_PROC_GEN || nd->op == IR_PROC_VALUE || ir_norm_call_kind(nd->op) == IR_CALL)
                    ? IR_LIT(nd).sval : (const char *)0;
    { extern int g_gva_active; extern int gva_index_of(const char *); int nm_op = (nd->op == IR_VAR || nd->op == IR_VAR_REF || nd->op == IR_ASSIGN);
      g_emit.op_gva_k = (g_gva_active && nm_op && IR_LIT(nd).sval && !graph_has_local(g_emit_cfg, IR_LIT(nd).sval)) ? gva_index_of(IR_LIT(nd).sval) : -1; }
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
    if (g_emit.op_a_slot < 0 && op_a) { int _z = nd_slot(op_a); if (_z >= 0) g_emit.op_a_slot = _z; }
    g_emit.op_a_counter = 0;
    g_emit.op_a_ival_sg = op_a ? IR_LIT(op_a).ival : 0;
    g_emit.op_a_dval = op_a ? IR_LIT(op_a).dval : 0;
    { extern int arith_emits_descr(IR_t *nd); g_emit.op_a_descr = arith_emits_descr(op_a) ? 1 : 0; }
    switch (nd->op) {
    case IR_LIT_INTEGER:
    case IR_LIT_STRING:
    case IR_LIT_CHARSET:
    case IR_LIT_REAL:               bb_emit_x86(bb_lit_scalar());         return 0;
    case IR_KEYWORD_ICON:
    case IR_KEYWORD_ICON_GEN:     bb_emit_x86(bb_keyword_icon());       return 0;
    case IR_KEYWORD_SNOBOL4:      bb_emit_x86(bb_keyword_snobol4());    return 0;
    case IR_KEYWORD_ASSIGN:       bb_emit_x86(bb_keyword_assign());     return 0;
    case IR_VAR:                  { extern int is_global(const char *);
        const char * _vn = IR_LIT(nd).sval;
        int _vn_reassignable_builtin = _vn && (!strcmp(_vn, "write") || !strcmp(_vn, "writes"));
        if (_vn && _vn[0] == '&') bb_emit_x86(bb_keyword_icon());
        else if (_vn && ((is_global(_vn) && !graph_has_local(g_emit_cfg, _vn)) || _vn_reassignable_builtin)) bb_emit_x86(bb_var_global());
        else bb_emit_x86(bb_var()); } return 0;
    case IR_VAR_REF:              bb_emit_x86(bb_var_ref());        return 0;
    case IR_COERCE_STRING:        { bb_prepare(nd); bb_emit_x86(bb_coerce_string()); }  return 0;
    case IR_COERCE_INTEGER:       { bb_prepare(nd); bb_emit_x86(bb_coerce_integer()); } return 0;
    case IR_COERCE_NUMERIC:       { bb_prepare(nd); bb_emit_x86(bb_coerce_numeric()); } return 0;
    case IR_COERCE_REAL:          { bb_prepare(nd); bb_emit_x86(bb_coerce_real()); }    return 0;
    case IR_CMP_TEST:             { bb_prepare(nd); bb_emit_x86(bb_cmp_test()); }      return 0;
    case IR_ASSIGN: {
        extern int is_global(const char *);
        const char * _an = IR_LIT(nd).sval;
        int _an_reassignable_builtin = _an && (!strcmp(_an, "write") || !strcmp(_an, "writes"));
        if (_an && ((is_global(_an) && !graph_has_local(g_emit_cfg, _an)) || _an_reassignable_builtin)) { bb_emit_x86(bb_assign_global()); return 0; }
        if (_an) { bb_emit_x86(bb_assign_local()); return 0; }
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
    case IR_MATCH_DEFER:          { bb_prepare(nd); bb_emit_x86(bb_match_defer()); } return 0;  /* SN4-PAT-FOLD deferred/stored pattern var */
    case IR_MATCH_ARBNO:          { extern int fc_tail_arbno(const IR_t *, int *, int *, int *, int *); extern int fc_tail_ncap(const IR_t *); int _fpb = 0, _fpl = 0, _osb = 0, _hdr = 0;
                                    if (ZC_FRAME == ZC_FRAME_RSP && fc_tail_arbno(nd, &_fpb, &_fpl, &_osb, &_hdr)) { g_emit.op_tail = 1; g_emit.op_sb = _osb; g_emit.op_sa = _hdr; g_emit.op_tail_fpb = _fpb; g_emit.op_tail_fpl = _fpl; g_emit.op_tail_ncap = fc_tail_ncap(nd); g_emit.op_tail_seal = (nd->n_operands > 3 && nd->operands[3] == nd) ? 1 : 0; }
                                    else { int _mo = -1, _sp = 0; if (zls_arbno_geom(nd, &_mo, &_sp)) { g_emit.op_sa = _mo; int _chain = (x86_port_mode() == ZC_PORT_FORTH); g_emit.op_arbno_chain = _chain; g_emit.op_sb = (_sp + (_chain ? 24 : 16) + 15) & ~15; } else { g_emit.op_sa = -1; g_emit.op_sb = -1; g_emit.op_arbno_chain = 0; } }
                                    bb_emit_x86(bb_match_arbno()); } return 0;   /* SN4-NARY-ARBNO: one node, flat-driven; R12-EXIT-1 carry-the-tail (rsp elements, op_sa = HDRB) or heap COLLECTION / ZB-FC-4 chain; geometry staged here */
    case IR_MATCH_HEAD:           { extern int fc_tail_head(const IR_t *); int _th = (ZC_FRAME == ZC_FRAME_RSP && fc_tail_head(nd)); if (fc_head_fp(nd) >= 0 || _th) { g_emit.op_fc_wbytes = 24; g_emit.op_fc_base = g_emit.op_off; } if (_th) g_emit.op_tail = 1; bb_emit_x86(bb_match_head()); } return 0;                  /* SN4-PAT-2 + ZB-FC-3d self-cell WINDOW + R12-EXIT-1 tail statements self-push too (bracket source) */
    case IR_MATCH_RELEASE:        { IR_t * _hd = nd->n_operands > 0 ? nd->operands[0] : (IR_t *)0; extern int fc_tail_release(const IR_t *, int *); int _br = -1;
                                    if (ZC_FRAME == ZC_FRAME_RSP && _hd && fc_tail_release(_hd, &_br)) { g_emit.op_tail = 1; g_emit.op_fc_disp = _br; }
                                    else g_emit.op_fc_disp = _hd ? fc_head_fp(_hd) : -1;
                                    bb_emit_x86(bb_match_release()); } return 0;               /* BB-OWNED-ζ statement-scope pivot + ZB-FC-3d cross-box cell reads + R12-EXIT-1 tail: bracket read off the TOP ELEMENT then unwind */
    case IR_MATCH_REPLACE:        { bb_emit_x86(bb_match_replace()); } return 0;               /* SN4-REPL stages 4/5 splice */
    case IR_MATCH_ASSIGN_COND:    { g_emit.op_fc_disp = fc_cond_fp(nd); bb_emit_x86(bb_match_capture()); } return 0;               /* SN4-PAT-2 + ZB-FC-3c cross-box read grant */
    case IR_MATCH_ASSIGN_IMM:     { g_emit.op_fc_disp = fc_cond_fp(nd); bb_emit_x86(bb_match_capture()); } return 0;               /* $ immediate capture + ZB-FC-3c (identical mechanism, phase 2) */
    case IR_MATCH_ASSIGN_SAVE:    { { long fck; if (fc_geom(nd, &fck)) { g_emit.op_fc_bytes = fck; g_emit.op_fc_base = g_emit.op_off; } } bb_emit_x86(bb_match_capture()); } return 0;               /* SN4-PAT-3h phase-0 SAVE + ZB-FC-3c fixed-cell grant (δ at cell+0) */
    case IR_MATCH_ALTERNATE:      { extern int fc_alt_fpmax(const IR_t *); extern int fc_alt_fp(const IR_t *, int);
                                    { long fck; if (fc_geom(nd, &fck)) { g_emit.op_fc_bytes = fck; g_emit.op_fc_base = g_emit.op_off; g_emit.op_fc_fpmax = fc_alt_fpmax(nd);
                                      for (int _j = 0; _j < (int)IR_LIT(nd).ival && _j < 16; _j++) g_emit.op_fc_arm_fp[_j] = fc_alt_fp(nd, _j); } }
                                    bb_emit_x86(bb_match_alternate()); } return 0;                   /* SN4-PAT-3h ALT + ZB-FC-3a linear-arm grant */
    case IR_MATCH_SEQUENCE:       { g_emit.op_fc_seq = (x86_port_mode() == ZC_PORT_FORTH && fc_seq_active(nd)) ? 1 : 0; bb_emit_x86(bb_match_sequence()); } return 0;   /* SN4-NARY-SEQ + ZB-FC-3b zero-LOCALS grant */
    case IR_SCAN_SEQUENCE:        { bb_emit_x86(bb_scan_sequence()); } return 0;   /* Icon scan concat: bb_match_sequence wiring, value = slice into own slot */
    case IR_SCAN_ALTERNATE:       { bb_emit_x86(bb_scan_alternate()); } return 0;  /* Icon scan alternation: bb_match_alternate wiring, value = slice into own slot */
    case IR_TO_BY:                { bb_prepare(nd); bb_emit_x86(bb_to_by()); } return 0;
    case IR_MAKE_LIST:            bb_emit_x86(bb_make_list());      return 0;
    case IR_CONJUNCTION:                 bb_emit_x86(bb_conjunction());           return 0;
    case IR_GOTO:                 { static int _mon = -1; if (_mon < 0) _mon = getenv("MONITOR_BIN") ? 1 : 0; extern void emit_mon_label_tap(int32_t); if (_mon && g_emit.op_stno > 0) emit_mon_label_tap(g_emit.op_stno); } bb_emit_x86(bb_goto());           return 0;
    case IR_GOTO_DEFERRED:             bb_emit_x86(bb_goto_dyn());       return 0;               /* EVAL/CODE runtime label transfer */
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
    case IR_INDIRECT_GOTO: case IR_DISJUNCTION: bb_emit_x86(bb_indirect_goto()); return 0;
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
        g_emit.op_sa = rv ? bb_slot_get(rv) : -1; g_emit.op_dval = IR_LIT(nd).dval; bb_emit_x86(bb_return()); return 0; }
    case IR_CALL_PROC_STAGED: case IR_CALL_BUILTIN: case IR_CALL_BUILTIN_GEN:
    case IR_CALL_BUILTIN_ICON: case IR_CALL_BUILTIN_SNOBOL4: case IR_CALL_BUILTIN_PROLOG:
    case IR_PROC_GEN:
    case IR_CALL: {
        g_emit.op_call_route = bb_call_route_classify(nd);
        bb_emit_x86(bb_call(nd));
        return 0;
    }
    case IR_FAIL:            bb_emit_x86(bb_fail());                            return 0;
    case IR_CUT:             bb_emit_x86(bb_cut());                             return 0;
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
#define DRIVE_FILL(nd,a,s,f,b) do { \
    g_emit.op_zls2_bytes = 0; g_emit.op_zls2_slot = -1; g_emit.op_zls2_ops = 0; g_emit.op_selfload = 0; \
    g_emit.op_fc_bytes = 0; g_emit.op_fc_base = -1; g_emit.x86_fc_synth = 240; g_emit.op_fc_fpmax = -1; g_emit.op_fc_seq = 0; g_emit.op_fc_disp = -1; g_emit.op_fc_wbytes = 0; g_emit.op_arbno_chain = 0; g_emit.op_tail = 0; g_emit.op_tail_fpb = 0; g_emit.op_tail_fpl = 0; g_emit.op_tail_seal = 0; g_emit.op_tail_ncap = 0; \
    g_emit.lbl_α=(a)->name; g_emit.lbl_γ=(s)->name; g_emit.lbl_ω=(f)->name; g_emit.lbl_β=(b)->name; \
    g_emit.lbl_α_p=(a); g_emit.lbl_γ_p=(s); g_emit.lbl_ω_p=(f); g_emit.lbl_β_p=(b); \
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
static void flat_drive_match_alt(IR_t **nodes, int n, int i, bb_label_t **lbls, bb_label_t **betas, bb_label_t **na_s, bb_label_t **na_f, bb_label_t ***fc_sig, bb_label_t *node_γ, bb_label_t *node_ω) {
    /* SN4-NARY-ALT drive: operands = (entry_j, resume_j) pairs.  Pair slots handed to the template:
     * 0..N-1 = entry α labels (fail-glue's alt_i dispatch), N..2N-1 = resume β labels (A.β's alt_i dispatch
     * — the alternative's OWN inner resume, seed alt_β), 2N = na_s success-glue define, 2N+1 = na_f
     * fail-glue define.  σ/φ-tagged inside edges land the two glue defines (resolved in the chain loop). */
    IR_t *nd = nodes[i]; int N = (nd->op == IR_MATCH_ARBNO) ? 1 : nd->n_operands / 2;   /* ARBNO: (entry,resume) + [2]=geometry bracket + optional [3]=self seal marker */
    g_emit.xa_bb_emit_pair_n = 0;
    for (int j = 0; j < N; j++) { IR_t *e = nd->operands[2 * j];     bb_label_t *t = node_ω; for (int k = 0; k < n; k++) if (nodes[k] == e) { t = lbls[k];  break; } int _i = g_emit.xa_bb_emit_pair_n++; g_emit.xa_bb_emit_pair_define[_i] = NULL; g_emit.xa_bb_emit_pair_jmp[_i] = t; }
    for (int j = 0; j < N; j++) { IR_t *r = nd->operands[2 * j + 1]; bb_label_t *t = node_ω; for (int k = 0; k < n; k++) if (nodes[k] == r) { t = betas[k]; break; } if (nd->op == IR_MATCH_ARBNO && nd->n_operands > 3 && nd->operands[3] == nd) t = na_f[i];   /* FENCE-rooted body: resume a committed iteration ≡ abandon it (seal) */ int _i = g_emit.xa_bb_emit_pair_n++; g_emit.xa_bb_emit_pair_define[_i] = NULL; g_emit.xa_bb_emit_pair_jmp[_i] = t; }
    { int _i = g_emit.xa_bb_emit_pair_n++; g_emit.xa_bb_emit_pair_define[_i] = na_s[i]; g_emit.xa_bb_emit_pair_jmp[_i] = NULL; }
    { int _i = g_emit.xa_bb_emit_pair_n++; g_emit.xa_bb_emit_pair_define[_i] = na_f[i]; g_emit.xa_bb_emit_pair_jmp[_i] = NULL; }
    /* ZB-FC-3a: per-arm sigma pad stubs (S10d pad-to-max).  Active iff LOWER registered the linear-arm fp set
     * AND the FORTH flavor is live; pairs 2N+2..3N+1 = stub defines, bodies emitted by the template.  The
     * fc_sig labels were allocated in the chain-body label loop; sigma edges into a granted ALT resolve to
     * stub[arm_of(source)] there.  Inactive => zero pairs pushed, zero routing, the pre-rung path verbatim. */
    if (nd->op == IR_MATCH_ALTERNATE && fc_alt_active(nd) && fc_sig && fc_sig[i])
        for (int j = 0; j < N; j++) { int _i = g_emit.xa_bb_emit_pair_n++; g_emit.xa_bb_emit_pair_define[_i] = fc_sig[i][j]; g_emit.xa_bb_emit_pair_jmp[_i] = NULL; }
    g_emit.op_off = drive_value_slot(nd); g_emit.op_ival = (int64_t)N;
    if (nd->op == IR_SCAN_SEQUENCE) { for (int j = 0; j < N && j < 32; j++) g_emit.op_parts_ival[j] = zls_off(nd->operands[2 * j + 1]); g_emit.op_parts_n = N; }   /* CV10: arm-value slots delivered to bb_scan_sequence (op_parts channel, IR_ALT precedent; 32-arm cap) */
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
    case IR_BINOP: case IR_BINOP_TEST: {
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
        DRIVE_PAIR_RESET(); DRIVE_PAIR_DEF_JMP(lbl_β, lbl_ω); DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_MATCH_HEAD: {
        IR_t *subj = nd->n_operands > 0 ? nd->operands[0] : (IR_t *)0;
        g_emit.op_sa = subj ? drive_value_slot(subj) : -1;
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
    case IR_MATCH_DEFER: {
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
    case IR_INDIRECT_GOTO: case IR_DISJUNCTION: {
        int off = drive_value_slot(nd);
        g_emit.op_off = off;
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_ITERATE: {
        IR_t * obj = nd->n_operands > 0 ? nd->operands[0] : NULL;
        int sa = obj ? bb_slot_get(obj) : -1;
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
        { extern int g_scan_regs_live; g_scan_regs_live++; }
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_SCAN: {
        g_emit.op_sb  = 0;
        { extern int g_scan_regs_live; if (g_scan_regs_live > 0) g_scan_regs_live--; }
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
    if (text_externalise && g_is_text) emit_label_define_bb(&lbl_α);
    xa_dispatch(XA_FLAT_PROLOGUE);
    if (g_is_text) g_emit_pos += 7;
    emit_label_define_bb(&lbl_α_body);
    enum { CH_MAX = 8192 };
    IR_t *nodes[CH_MAX]; int n = 0;
    IR_t *queue[CH_MAX]; int qh = 0, qt = 0;
    { int guard = 0; while (entry && (entry->op == IR_SUCCEED || entry->op == IR_FAIL) && entry->γ.node && guard++ < CH_MAX) entry = entry->γ.node; }
    /* PROC-CONV: an EMPTY body (bare `:(RETURN)` / `:(FRETURN)` — the terminal is a childless SUCCEED/FAIL, so
     * the BFS below collects ZERO nodes) must not let lbl_α_body FALL THROUGH into the jmp-entry lbl_res landing
     * (`add rsp,8; pop zr`), which corrupts rsp/zr on the initial α pass — res is reachable ONLY via the outside
     * `jmp qword [rsp+0]` backtrack.  A non-empty body's last node always JMPs to lbl_γ/lbl_ω (both defined AFTER
     * res), so it never falls in; only n==0 does.  Record the terminal so α_body can jump straight to it. */
    int flat_empty_body_fail = (entry && entry->op == IR_FAIL) ? 1 : 0;
    int flat_empty_body_succ = (entry && entry->op == IR_SUCCEED) ? 1 : 0;
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
        if ((c->op == IR_BINOP || c->op == IR_BINOP_TEST || c->op == IR_UNOP || c->op == IR_UNOP_TEST || c->op == IR_NULLTEST_VAR || c->op == IR_COERCE_STRING || c->op == IR_COERCE_INTEGER || c->op == IR_COERCE_NUMERIC || c->op == IR_COERCE_REAL || c->op == IR_CMP_TEST) && c->ω.node && qt < CH_MAX) queue[qt++] = c->ω.node;
        if ((c->op == IR_CALL || ir_is_call_kind(c->op) || c->op == IR_PROC_GEN || c->op == IR_ACTIVATE) && c->ω.node && qt < CH_MAX) queue[qt++] = c->ω.node;
        if ((c->op == IR_SUBSCRIPT || c->op == IR_RANDOM || c->op == IR_DEREF || c->op == IR_ASSIGN_VAR || c->op == IR_REV_ASSIGN_VAR || c->op == IR_KEYWORD_ASSIGN || c->op == IR_SCAN_TAB || c->op == IR_SCAN_MOVE || c->op == IR_SCAN_POS || c->op == IR_SCAN_MATCH || c->op == IR_SCAN_ANY
             || c->op == IR_SWAP_VAR || c->op == IR_CALL_VALUE || c->op == IR_VAR) && c->ω.node && qt < CH_MAX)
            queue[qt++] = c->ω.node;
        if (c->op == IR_SUSPEND && c->n_operands > 1 && c->operands[1] && qt < CH_MAX) queue[qt++] = c->operands[1];
        if (c->op == IR_CREATE && c->n_operands > 0 && c->operands[0] && qt < CH_MAX) queue[qt++] = c->operands[0];
        if (c->op == IR_MOVE_LABEL && c->n_operands > 0 && c->operands[0] && qt < CH_MAX) queue[qt++] = c->operands[0];
        if (c->op == IR_MOVE_LABEL && c->n_operands > 1 && c->operands[1] && qt < CH_MAX) queue[qt++] = c->operands[1];
        if (c->op == IR_REPALT && c->n_operands > 0 && c->operands[0] && qt < CH_MAX) queue[qt++] = c->operands[0];
        if (c->op == IR_REPALT && c->n_operands > 1 && c->operands[1] && qt < CH_MAX) queue[qt++] = c->operands[1];
        if (((c->op >= IR_MATCH_LIT && c->op <= IR_MATCH_ASSIGN_SAVE) || c->op == IR_MATCH_DEFER) && c->ω.node && qt < CH_MAX) queue[qt++] = c->ω.node;  /* SN4-PAT: match-family ω is a real control edge (next alternative / fail handler; DEFER folded in for SN4-PAT-FOLD) */
        if ((c->op == IR_MATCH_ALTERNATE || c->op == IR_MATCH_SEQUENCE || c->op == IR_MATCH_ARBNO || c->op == IR_SCAN_SEQUENCE || c->op == IR_SCAN_ALTERNATE) && c->n_operands > 0) for (int _oi = 0; _oi < c->n_operands; _oi++) if (c->operands[_oi] && qt < CH_MAX) queue[qt++] = c->operands[_oi];   /* SN4-NARY-ALT/-SEQ: alternatives/elements hang off operands, not the γ-spine (BOTH copies of the dup walk — the s31 dedup finding stands) */
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
        if ((c->op == IR_BINOP || c->op == IR_BINOP_TEST || c->op == IR_UNOP || c->op == IR_UNOP_TEST || c->op == IR_NULLTEST_VAR || c->op == IR_COERCE_STRING || c->op == IR_COERCE_INTEGER || c->op == IR_COERCE_NUMERIC || c->op == IR_COERCE_REAL || c->op == IR_CMP_TEST) && c->ω.node && qt < CH_MAX) queue[qt++] = c->ω.node;
        if ((c->op == IR_CALL || ir_is_call_kind(c->op) || c->op == IR_PROC_GEN || c->op == IR_ACTIVATE) && c->ω.node && qt < CH_MAX) queue[qt++] = c->ω.node;
        if ((c->op == IR_SUBSCRIPT || c->op == IR_RANDOM || c->op == IR_DEREF || c->op == IR_ASSIGN_VAR || c->op == IR_REV_ASSIGN_VAR || c->op == IR_KEYWORD_ASSIGN || c->op == IR_SCAN_TAB || c->op == IR_SCAN_MOVE || c->op == IR_SCAN_POS || c->op == IR_SCAN_MATCH || c->op == IR_SCAN_ANY
             || c->op == IR_SWAP_VAR || c->op == IR_CALL_VALUE || c->op == IR_VAR) && c->ω.node && qt < CH_MAX)
            queue[qt++] = c->ω.node;
        if (ir_is_generator_kind(c->op) && c->ω.node && qt < CH_MAX) queue[qt++] = c->ω.node;
        if (((c->op >= IR_MATCH_LIT && c->op <= IR_MATCH_ASSIGN_SAVE) || c->op == IR_MATCH_DEFER) && c->ω.node && qt < CH_MAX) queue[qt++] = c->ω.node;  /* SN4-PAT: match-family ω is a real control edge (next alternative / fail handler; DEFER folded in for SN4-PAT-FOLD) */
        if (c->op == IR_SUSPEND && c->n_operands > 1 && c->operands[1] && qt < CH_MAX) queue[qt++] = c->operands[1];
        if (c->op == IR_CREATE && c->n_operands > 0 && c->operands[0] && qt < CH_MAX) queue[qt++] = c->operands[0];
        if (c->op == IR_MOVE_LABEL && c->n_operands > 0 && c->operands[0] && qt < CH_MAX) queue[qt++] = c->operands[0];
        if (c->op == IR_MOVE_LABEL && c->n_operands > 1 && c->operands[1] && qt < CH_MAX) queue[qt++] = c->operands[1];
        if (c->op == IR_REPALT && c->n_operands > 0 && c->operands[0] && qt < CH_MAX) queue[qt++] = c->operands[0];
        if (c->op == IR_REPALT && c->n_operands > 1 && c->operands[1] && qt < CH_MAX) queue[qt++] = c->operands[1];
        if ((c->op == IR_MATCH_ALTERNATE || c->op == IR_MATCH_SEQUENCE || c->op == IR_MATCH_ARBNO || c->op == IR_SCAN_SEQUENCE || c->op == IR_SCAN_ALTERNATE) && c->n_operands > 0) for (int _oi = 0; _oi < c->n_operands; _oi++) if (c->operands[_oi] && qt < CH_MAX) queue[qt++] = c->operands[_oi];   /* SN4-NARY-ALT/-SEQ: alternatives/elements hang off operands, not the γ-spine (BOTH copies of the dup walk — the s31 dedup finding stands) */
    }
    if (qt >= CH_MAX) { fprintf(stderr, "[GZ-7] FATAL: chain traversal queue saturated (qt=%d >= CH_MAX=%d) for prefix=%s -- control-flow edges were silently dropped; raise CH_MAX\n", qt, (int)CH_MAX, prefix); abort(); }
    bb_label_t **lbls  = (bb_label_t **)alloca(sizeof(bb_label_t *) * n);
    bb_label_t **betas = (bb_label_t **)alloca(sizeof(bb_label_t *) * n);
    bb_label_t **ra_y  = (bb_label_t **)alloca(sizeof(bb_label_t *) * n);
    bb_label_t **ra_t  = (bb_label_t **)alloca(sizeof(bb_label_t *) * n);
    bb_label_t **na_s  = (bb_label_t **)alloca(sizeof(bb_label_t *) * n);
    bb_label_t ***fc_sig = (bb_label_t ***)alloca(sizeof(bb_label_t **) * n);
    bb_label_t **na_f  = (bb_label_t **)alloca(sizeof(bb_label_t *) * n);
    for (int i = 0; i < n && g_flat_chain_set_n < FLAT_CHAIN_SET_MAX; i++) g_flat_chain_set[g_flat_chain_set_n++] = nodes[i];
    g_suspend_resume_slot = -1;
    if (g_gen_proc_active && g_emit_cfg && g_emit_cfg->resume_slot >= 0)
        for (int _si = 0; _si < n; _si++) if (nodes[_si]->op == IR_SUSPEND) { g_suspend_resume_slot = g_emit_cfg->resume_slot; break; }
    if (g_suspend_resume_slot < 0 && g_resumable_callable_active && g_emit_cfg && g_emit_cfg->resume_slot >= 0)
        g_suspend_resume_slot = g_emit_cfg->resume_slot;
    int id = g_flat_node_id++;
    /* RUNG ZB-OWN-0 (Lon 2026-07-11): per-chain statement mark = HEAD's zls entry +16 (head.zls2_mark quad);
     * -1 when the chain has no HEAD (non-match chains carry no per-box shadow at rung 0 -- recorded scope cut). */
    int own_mark = -1;
    { extern int zls_off(const IR_t *); for (int _oi = 0; _oi < n; _oi++) if (nodes[_oi]->op == IR_MATCH_HEAD) { int _ho = zls_off(nodes[_oi]); if (_ho >= 0) own_mark = _ho + 16; break; } }
    for (int i = 0; i < n; i++) {
        lbls[i]  = emit_label_alloc("xchain%d_n%d_α", id, i);
        betas[i] = emit_label_alloc("xchain%d_n%d_β", id, i);
        ra_y[i]  = (nodes[i]->op == IR_REPALT) ? emit_label_alloc("xchain%d_n%d_ry", id, i) : NULL;
        ra_t[i]  = (nodes[i]->op == IR_REPALT) ? emit_label_alloc("xchain%d_n%d_rt", id, i) : NULL;
        na_s[i]  = ((nodes[i]->op == IR_MATCH_ALTERNATE || nodes[i]->op == IR_MATCH_SEQUENCE || nodes[i]->op == IR_MATCH_ARBNO || nodes[i]->op == IR_SCAN_SEQUENCE || nodes[i]->op == IR_SCAN_ALTERNATE) && nodes[i]->n_operands > 0) ? emit_label_alloc("xchain%d_n%d_as", id, i) : NULL;   /* SN4-NARY-ALT/-SEQ success-glue */
        na_f[i]  = ((nodes[i]->op == IR_MATCH_ALTERNATE || nodes[i]->op == IR_MATCH_SEQUENCE || nodes[i]->op == IR_MATCH_ARBNO || nodes[i]->op == IR_SCAN_SEQUENCE || nodes[i]->op == IR_SCAN_ALTERNATE) && nodes[i]->n_operands > 0) ? emit_label_alloc("xchain%d_n%d_af", id, i) : NULL;   /* SN4-NARY-ALT/-SEQ fail-glue */
        fc_sig[i] = NULL;   /* ZB-FC-3a: per-arm sigma pad-stub labels for a LINEAR-ARM granted ALT (FORTH flavor) */
        if (fc_alt_active(nodes[i])) {
            int _N = (int)(nodes[i]->n_operands / 2);
            fc_sig[i] = (bb_label_t **)alloca(sizeof(bb_label_t *) * _N);
            for (int _j = 0; _j < _N; _j++) fc_sig[i][_j] = emit_label_alloc("xchain%d_n%d_s%d", id, i, _j);
        }
    }
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
                resume_init_lbl->name, x86_zr(), g_suspend_resume_slot);
            emit_text_n(_init, strlen(_init));
        } else {
            ef_b3(0x48, 0x8D, 0x05); bb_emit_patch_rel32(resume_init_lbl);
            { int z = x86_zr_num(), lo = z & 7; ef_b2((uint8_t)(0x48 | (z >= 8 ? 0x01 : 0x00)), 0x89); if (lo == 4) ef_b2(0x84, 0x24); else ef_b1((uint8_t)(0x80 | lo)); bb_emit_u32((uint32_t)(unsigned)g_suspend_resume_slot); }
        }
    }
    for (int _li = 0; _li < n; _li++) if (nodes[_li]->op == IR_LIMIT) {
        g_emit.op_off = drive_value_slot(nodes[_li]);
        bb_emit_x86(bb_limit_init());
    }
    for (int i = 0; i < n; i++) {
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
        { extern int zls_off(const IR_t *); extern int zls_node_bytes(const IR_t *); int _zo = zls_off(nodes[i]);
          g_emit.op_own_mark = own_mark; g_emit.op_own_ci = (_zo >= 0) ? _zo + zls_node_bytes(nodes[i]) : 0; }
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
            if (gamma_is_sig && fc_sig[k] && fc_alt_active(nodes[k])) {
                /* arm_of(source) = the arm whose entry is NEAREST AT-OR-BELOW pos(source).  The prior "largest j with pos(entry_j) <= pos(source)" assumed arm entries allocate ASCENDING, but
                 * right-first lowering allocates them DESCENDING (arm N-1's nodes get the LOWEST indices), so every source at-or-after entry_0 spuriously matched the last passing j — measured s71:
                 * SPAN('a')|'b' routed SPAN's yield (already at full arm depth) through the literal arm's +16 pad stub, desyncing every later [rsp+const] until rsp died.  Equal-fp arms (163's
                 * literal|literal, all stubs identical) masked it.  Nearest-below is order-agnostic: each arm's nodes sit contiguously at-or-after its own entry. */
                int _N = (int)(nodes[k]->n_operands / 2), _arm = 0, _src = -1, _best = -1;
                for (int _p = 0; _p < n; _p++) if (nodes[_p] == nodes[i]) { _src = _p; break; }
                for (int _j = 0; _j < _N; _j++) { IR_t *_e = nodes[k]->operands[2 * _j]; for (int _p = 0; _p < n; _p++) if (nodes[_p] == _e) { if (_p <= _src && _p > _best) { _best = _p; _arm = _j; } break; } }
                node_γ = fc_sig[k][_arm];
            }
            if (gamma_is_sig && fc_seq_on(nodes[k])) node_γ = fc_seq_sigma_tgt(nodes, n, k, i, lbls,  node_γ);   /* ZB-FC-3b: inside-success → NEXT element's α (static; rsp already correct) */
            if (gamma_is_phi && fc_seq_on(nodes[k])) node_γ = fc_seq_phi_tgt  (nodes, n, k, i, betas, node_γ);   /* ZB-FC-3b: a FAIL-goto's γ means fail → PREV element's β */
            break;
        }
        if (nodes[i]->γ.node == NULL || nodes[i]->γ.node->op == IR_SUCCEED) node_γ = &lbl_γ;
        if (nodes[i]->γ.node && nodes[i]->γ.node->op == IR_FAIL) node_γ = &lbl_ω;
        int omega_resolved = 0;
        for (int k = 0; k < n; k++) if (nodes[k] == otgt) { node_ω = (omega_is_phi && na_f[k]) ? na_f[k] : omega_is_beta ? betas[k] : lbls[k]; if (omega_is_phi && fc_seq_on(nodes[k])) node_ω = fc_seq_phi_tgt(nodes, n, k, i, betas, node_ω); omega_resolved = 1; break; }   /* ZB-FC-3b: inside-fail → PREV element's β (static; subtree already popped) */
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
            else { for (int k = 0; k < n; k++) if (nodes[k] == dobody) { g_suspend_dobody_beta = ir_is_generator_kind(dobody->op) ? betas[k] : lbls[k]; break; } }
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
        if ((nodes[i]->op == IR_MATCH_ALTERNATE || nodes[i]->op == IR_MATCH_SEQUENCE || nodes[i]->op == IR_MATCH_ARBNO || nodes[i]->op == IR_SCAN_SEQUENCE || nodes[i]->op == IR_SCAN_ALTERNATE) && nodes[i]->n_operands > 0) {
            flat_drive_match_alt(nodes, n, i, lbls, betas, na_s, na_f, fc_sig, node_γ, node_ω);   /* SN4-NARY-SEQ rides the identical (entry,resume)×N + 2-glue pair layout */
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
        emit_drive(nodes[i], lbls[i], node_γ, node_ω, betas[i]);
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
        emit_label_define_bb(&lbl_res);
        if (g_is_text) {
            char _res[96];
            snprintf(_res, sizeof _res, "add rsp, 8\npop %s\n", x86_zr());   /* REG-5 s79: the record[+8] re-pin routes through the selector — rbp for pat blobs, never rsp */
            emit_text_n(_res, strlen(_res));
        } else {
            int z = x86_zr_num();
            ef_b4(0x48, 0x83, 0xC4, 0x08);
            if (z >= 8) ef_b2(0x41, (uint8_t)(0x58 | (z & 7))); else ef_b1((uint8_t)(0x58 | (z & 7)));
        }
    }
    emit_label_define_bb(&lbl_β);
    if (g_suspend_resume_slot >= 0 && (g_gen_proc_active || g_resumable_callable_active)) {
        if (g_is_text) {
            char _ind_jmp[64];
            snprintf(_ind_jmp, sizeof _ind_jmp, "jmp qword ptr [%s + %d]\n", x86_zr(), g_suspend_resume_slot);
            emit_text_n(_ind_jmp, strlen(_ind_jmp));
        } else {
            { int z = x86_zr_num(), lo = z & 7; ef_b2((uint8_t)(0x48 | (z >= 8 ? 0x01 : 0x00)), 0xFF); if (lo == 4) ef_b2(0xA4, 0x24); else ef_b1((uint8_t)(0xA0 | lo)); bb_emit_u32((uint32_t)(unsigned)g_suspend_resume_slot); }
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
    int rg = g ? zls_g_region(g) : -1;
    if (rg <= 0) rg = 4096;
    g_emit.flat_jmp_entry = 1; g_emit.flat_frame_bytes = (32 + rg + 15) & ~15;
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
extern "C" void emit_jmp_entry_clear(void) { g_emit.flat_jmp_entry = 0; g_emit.flat_frame_bytes = 0; g_emit.flat_pat = 0; }
/* PROC-CONV (R12-FREE ladder rung 2): ordinary DEFINE'd procs arm the SAME jmp-entry regime.  The regime
 * selector is dyn_scope: a DYN proc's args ride the name dictionary (save/restore), so a self-allocated zeroed
 * frame is correct; a LEXICAL proc (dyn_scope=0 — Icon/Prolog/Raku frontends) binds args INTO a caller-made
 * frame via rt_frame_prep, which jmp-entry has no place for — those stay call-regime until NCB-1d converts the
 * static-link family.  LBL__ main-program pseudo-procs are dyn_scope=0 but take no args and are entered only
 * through rt_chain_enter (rt_goto_transfer arm 4), which IS the jmp transfer — they arm by name. */
extern "C" int emit_jmp_entry_for_proc(const char *pname, int dyn_scope, int is_generator, IR_graph_t *g) {
    if (is_generator) return 0;
    if (!dyn_scope && !(pname && strncmp(pname, "LBL__", 5) == 0)) return 0;
    return emit_jmp_entry_arm_region(g);
}
/* s60 EVAL/CODE-RSP (Lon ruling s59: EVAL/CODE = the DEFER shape): runtime-compiled EVAL statement chains and
 * CODE fragments speak the same jmp-entry protocol — self-allocating rsp activation, 32B wire header, wired
 * outside-γ/ω, jump back, never call/ret.  runtime_eval.c brackets its emit_chain calls with this pair and
 * enters through rt_chain_enter (the C-side resolve→wire→jmp).  Same K_total formula as the PAT$ gate above. */
extern "C" int emit_jmp_entry_for_chain(IR_graph_t *g) {
    return emit_jmp_entry_arm_region(g);
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
#ifdef __cplusplus
}
#endif

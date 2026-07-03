/* emit.cpp - THE single SCRIP emitter translation unit.
 * Consolidated from emit_core.c + emit_globals.c + emit_drive.c (GOAL-IR-IMMUTABLE-EMIT).
 * ONE file, ONE per-node driver (emit_drive). Templates (BB_/XA_*.cpp) are untouched.
 * C++ TU because walk_bb_node selects C++ x86 templates; emitter symbols keep C linkage
 * via the extern "C" block so C callers (driver/lowerer/opt) link unchanged. */
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
/*--- emitter mutable state instance (merged from emit_globals.c) ---*/
sm_emit_t g_emit;
IR_graph_t * g_emit_cfg = (IR_graph_t *)0;
/*============================================= OUTPUT SINK & LABEL POOL =============================================*/
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
void emit_label_pool_reset(void)
{
    for (int i = 0; i < g_label_pool_n; i++) free(g_label_pool[i]);
    g_label_pool_n = 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------------------------------------------------*/
bb_label_t *emit_label_intern(const char *name)
{
    if (!name) return NULL;
    for (int i = 0; i < g_label_pool_n; i++)
        if (g_label_pool[i] && strcmp(g_label_pool[i]->name, name) == 0) return g_label_pool[i];
    return emit_label_alloc("%s", name);
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*===================================== BYTE EMISSION & EMITTER INIT / TEARDOWN ======================================*/
void bb_emit_begin(bb_buf_t buf, int size)
{
    bb_emit_buf    = buf;
    bb_emit_pos    = 0;
    bb_emit_size   = size;
    bb_patch_count = 0;
    emit_label_pool_reset();
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern int bb_emit_overflow;
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
/*--------------------------------------------------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------------------------------------------------*/
int bb_emit_overflow = 0;
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
/*--------------------------------------------------------------------------------------------------------------------*/
void  bb_emit_u32(uint32_t v)  { bb_emit_byte((uint8_t)(v)); bb_emit_byte((uint8_t)(v>>8)); bb_emit_byte((uint8_t)(v>>16)); bb_emit_byte((uint8_t)(v>>24)); }
void  bb_emit_u64(uint64_t v)  { bb_emit_u32((uint32_t)(v)); bb_emit_u32((uint32_t)(v >> 32)); }
void  bb_emit_i32(int32_t v)   { uint32_t u; memcpy(&u, &v, 4); bb_emit_u32(u); }
int  g_is_text        = 0;
int  g_emit_text_mode = TEXT_MODE_INVOCATION;
int  g_emit_pos       = 0;
void emitter_init_binary(bb_buf_t buf, int size)
{
    g_is_text = 0; g_emit_text_mode = TEXT_MODE_INVOCATION; g_emit_pos = 0;
    bb_emit_overflow = 0;
    bb_emit_mode = EMIT_BINARY_WIRED;
    g_platform = BB_PLATFORM_X86; g_medium = BB_MEDIUM_BINARY;
    bb_emit_begin(buf, size);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void emitter_init_text(FILE *out, int mode)
{
    g_is_text = 1; g_emit_text_mode = mode; g_emit_pos = 0;
    bb_emit_mode = EMIT_TEXT;
    g_platform = BB_PLATFORM_X86; g_medium = BB_MEDIUM_TEXT;
    bb_emit_out  = out ? out : stdout;
    emit_io_set_sink(out ? out : stdout);
}
/*--------------------------------------------------------------------------------------------------------------------*/
int  emitter_end(void)        { return g_is_text ? g_emit_pos : bb_emit_end(); }
static void  ef_b1 (uint8_t a)                                   { bb_emit_byte(a); }
static void  ef_b2 (uint8_t a, uint8_t b)                        { bb_emit_byte(a); bb_emit_byte(b); }
static void  ef_b3 (uint8_t a, uint8_t b, uint8_t c)             { bb_emit_byte(a); bb_emit_byte(b); bb_emit_byte(c); }
static void  ef_b4 (uint8_t a, uint8_t b, uint8_t c, uint8_t d)  { bb_emit_byte(a); bb_emit_byte(b); bb_emit_byte(c); bb_emit_byte(d); }
static void ef_t3c_jmp(const char *mnem, const char *target)
{ fprintf(bb_emit_out, "%s %s\n", mnem ? mnem : "", target ? target : ""); }
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
/*--------------------------------------------------------------------------------------------------------------------*/
void emit_jmp_label(bb_label_t *target, jmp_kind_t kind)
{
    static const char    *mn[]    = {"jmp","je","jne","jl","jge","jg"};
    static const uint8_t  ops[6][2] = {{0xE9,0x00},{0x0F,0x84},{0x0F,0x85},{0x0F,0x8C},{0x0F,0x8D},{0x0F,0x8F}};
    int k = (int)kind < 6 ? (int)kind : 0;
    if (g_is_text) { ef_t3c_jmp(mn[k], target->name); g_emit_pos += 6; }
    else { if (k==0) ef_b1(0xE9); else ef_b2(ops[k][0], ops[k][1]); bb_emit_patch_rel32(target); }
}
/*--------------------------------------------------------------------------------------------------------------------*/
void emit_aligned_call_rt(const char *sym, void *addr)
{
    if (g_is_text) { fprintf(bb_emit_out, " push rbx\n mov rbx, rsp\n and rsp, -16\n call %s@PLT\n mov rsp, rbx\n pop rbx\n", sym ? sym : ""); }
    else { ef_b1(0x53); ef_b3(0x48,0x89,0xE3); ef_b4(0x48,0x83,0xE4,0xF0); ef_b2(0x48,0xB8); bb_emit_u64((uint64_t)(uintptr_t)addr); ef_b2(0xFF,0xD0); ef_b3(0x48,0x89,0xDC); ef_b1(0x5B); }
}
/*--------------------------------------------------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------------------------------------------------*/
void emit_label_initf(bb_label_t *lbl, const char *fmt, ...)
{
    va_list ap; va_start(ap, fmt);
    vsnprintf(lbl->name, BB_LABEL_NAME_MAX, fmt, ap);
    va_end(ap);
    lbl->offset = BB_LABEL_UNRESOLVED;
}
/*--------------------------------------------------------------------------------------------------------------------*/
#include "IR.h"
#include "emit_ir.h"
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_emit_limit_init(int limit_slot_off) { fprintf(stderr, "GROUND ZERO: %s not implemented (Icon-only reset)\n", "bb_emit_limit_init"); abort(); }
/*--------------------------------------------------------------------------------------------------------------------*/
#define WASM_STRTAB_MAX 4096
#define WASM_STR_DATA_BASE 0x100000
typedef struct { const char * s; int addr; int len; } WasmStrEntry;
static WasmStrEntry g_wasm_strtab[WASM_STRTAB_MAX];
static int g_wasm_strtab_n = 0;
static int g_wasm_str_next = WASM_STR_DATA_BASE;
/*--------------------------------------------------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C++" {
#endif
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
/*--------------------------------------------------------------------------------------------------------------------*/
#ifdef __cplusplus
}
/*--------------------------------------------------------------------------------------------------------------------*/
#endif
#define WASM_USERFNS_MAX 256
#define WASM_MAX_PARAMS  16
typedef struct { char name[128]; int entry_pc; int nparams; char params[WASM_MAX_PARAMS][128]; } WasmUserFn;
static WasmUserFn g_wasm_userfns[WASM_USERFNS_MAX];
static int        g_wasm_userfns_n = 0;
/*--------------------------------------------------------------------------------------------------------------------*/
int g_m4_dense_nid = 0;
static IR_t * g_nid_key[262144];
static int    g_nid_val[262144];
static int    g_nid_count = 0;
int bb_node_id(IR_t * nd) {
    if (!nd) return 0;
    if (!g_m4_dense_nid) return (int)((uintptr_t)nd % 100000u);
    uintptr_t h = ((uintptr_t)nd >> 4) & 262143u;
    while (g_nid_key[h]) { if (g_nid_key[h] == nd) return g_nid_val[h]; h = (h + 1u) & 262143u; }
    g_nid_key[h] = nd; g_nid_val[h] = ++g_nid_count; return g_nid_val[h];
}
/*--------------------------------------------------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------------------------------------------------*/
#define SMX_STRTAB_CAP 8192
static struct { const char *s; int idx; } g_strtab[SMX_STRTAB_CAP];
static int g_strtab_n = 0;
/*============================================= STRING TABLE (xa rodata) =============================================*/
void strtab_reset(void) { g_strtab_n = 0; }
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
/*--------------------------------------------------------------------------------------------------------------------*/
void strtab_label(char *buf, size_t bufsz, const char *s)
{
    if (!s) s = "";
    int idx = strtab_intern(s);
    snprintf(buf, bufsz, ".S%d", idx);
}
/*--------------------------------------------------------------------------------------------------------------------*/
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
/*========================================= SLOT MACHINERY & IR NODE HELPERS =========================================*/
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
static void bb_slotmap_push(IR_t *nd, int off) {
    if (g_bb_slotmap_n >= g_bb_slotmap_max) {
        int new_max = g_bb_slotmap_max ? g_bb_slotmap_max * 2 : 512;
        bb_slotmap_ent_t *g = (bb_slotmap_ent_t *)realloc(g_bb_slotmap, (size_t)new_max * sizeof(bb_slotmap_ent_t));
        if (!g) return;
        g_bb_slotmap = g; g_bb_slotmap_max = new_max;
    }
    g_bb_slotmap[g_bb_slotmap_n].key = nd; g_bb_slotmap[g_bb_slotmap_n].off = off; g_bb_slotmap_n++;
}
int bb_slot_get(IR_t *nd) {
    for (int i = 0; i < g_bb_slotmap_n; i++) if (g_bb_slotmap[i].key == nd) return g_bb_slotmap[i].off;
    return -1;
}
void bb_slot_register(IR_t *nd, int off) {
    bb_slotmap_push(nd, off);
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/* TE-4 VARSLOT ABSORPTION (Lon directive 2026-07-02): the emit-time name→slot bump allocator is DELETED —
   LOWER's ir_drive_slot_assign interns every param + local + the gen-proc resume cell into the graph's vslots
   table (identical layout: params ABI-fixed, locals above the tmp region), and this peek is a pure READ of
   that table through g_emit_cfg. A local with no LOWER-granted slot is a LOWER gap: the IR_ASSIGN /
   IR_REV_ASSIGN drive arms abort loud, never allocate here (the drive_value_slot doctrine). */
int bb_varslot_peek(const char *name) {
    return ir_varslot_of(g_emit_cfg, name);
}
/*--------------------------------------------------------------------------------------------------------------------*/
int g_proc_direct_active = 0;
int g_gvar_flat_chain = 0;
int g_gva_active = 0;
int g_gvar_callarg_live = 0;
int g_emit_frame_caller_dl = -1;
int g_frame_active = 0;
int g_gen_proc_active = 0;
int g_scan_regs_live = 0;
/*--------------------------------------------------------------------------------------------------------------------*/
#define FLAT_CHAIN_SET_MAX 512
static IR_t *g_flat_chain_set[FLAT_CHAIN_SET_MAX];
static int   g_flat_chain_set_n = 0;
static bb_label_t *g_limit_gen_beta = NULL;   /* chain hands flat_drive_limit its generator's resume β */
static bb_label_t *g_create_body_entry = NULL; /* RUNG 3b: chain hands bb_create its coexpression body-entry α-label as a bb_label_t* (threaded through the t0 port so binary-mode's LEA is rel32-patchable — LIMITATION 2's fix, mirroring g_limit_gen_beta) */
static bb_label_t *g_move_label_tgt = NULL; /* unbounded alternation: chain hands bb_move_label the arm's resume-point label (β of the arm's generator when IR_LIT(ml).ival==1, α of the arm's failure continuation when 0 — JCON eList[i].resume decoded), threaded through the t0 port exactly like g_create_body_entry */
static bb_label_t *g_suspend_dobody_beta = NULL; /* chain hands flat_drive_suspend its do-body resume label */
static int          g_suspend_resume_slot = -1;        /* byte offset in gen-proc frame for indirect-goto resume pointer */
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
/*--------------------------------------------------------------------------------------------------------------------*/
const char *child_cache_get_lbl(bb_box_fn fn) { fprintf(stderr, "GROUND ZERO: %s not implemented (Icon-only reset)\n", "child_cache_get_lbl"); abort(); }
/*--------------------------------------------------------------------------------------------------------------------*/
int    g_flat_data_any    = 0;
static int    g_flat_data_just_closed = 0;
static char   g_flat_data_pending_lbl[160] = "";
static char   g_flat_data_block_lbls[FLAT_DATA_LBL_MAX][96];
static int    g_flat_data_block_nlbls = 0;
/*--------------------------------------------------------------------------------------------------------------------*/
void data_buf_reset(void) { g_flat_data_len = 0; g_flat_data_active = 0; g_flat_data_any = 0; g_flat_data_just_closed = 0; g_flat_data_block_nlbls = 0; g_flat_data_pending_lbl[0] = '\0'; }
/*--------------------------------------------------------------------------------------------------------------------*/
static void data_buf_appendf(const char *fmt, ...) {
    if (g_flat_data_len >= FLAT_DATA_BUF_MAX) return; va_list ap; va_start(ap, fmt);
    int n = vsnprintf(g_flat_data_buf + g_flat_data_len, FLAT_DATA_BUF_MAX - g_flat_data_len, fmt, ap); va_end(ap);
    if (n > 0) { size_t left = FLAT_DATA_BUF_MAX - g_flat_data_len; g_flat_data_len += ((size_t)n < left) ? (size_t)n : left; } }
/*--------------------------------------------------------------------------------------------------------------------*/
void data_buf_flush_pending_label(void) { if (!g_flat_data_pending_lbl[0]) return; data_buf_appendf("%s\n", g_flat_data_pending_lbl); g_flat_data_pending_lbl[0] = '\0'; }
#define SYM_SIGMA   "\xCE\xA3"
#define SYM_SIGLEN  "\xCE\xA3""len"
#define SYM_DELTA   "\xCE\x94"
#define ADDR_SIGMA   ((uint64_t)(uintptr_t)&Σ)
#define ADDR_SIGLEN  ((uint64_t)(uintptr_t)&Σlen)
#define ADDR_DELTA   ((uint64_t)(uintptr_t)&Δ)
static const char *(*g_flat_intern_str)(const char *s) = NULL;
/*--------------------------------------------------------------------------------------------------------------------*/
const char *emit_intern_str(const char *s) { (void)s; return NULL; }
void walk_bb_flat(IR_t *nd, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β);
static void flat_emit_arg_subchain(IR_t *entry, bb_label_t *succ, bb_label_t *fail);
static void descr_chain_operand_refs(IR_t *entry);
static void gvar_stmt_operand_refs(IR_t *head);
static int gvar_prewalk_idx_operand(IR_t *idx, bb_label_t *lbl_ω);
IR_t * bb_child0(const IR_t *n) { return (n && n->n_operands > 0) ? n->operands[0] : NULL; }
IR_t * bb_child1(const IR_t *n) { return (n && n->n_operands > 1) ? n->operands[1] : NULL; }
extern int memcmp(const void *, const void *, size_t);
static bb_label_t g_α_ring[8];
static int        g_α_ring_i = 0;
static int        g_bb_alpha_seq = 0;void g_bb_alpha_seq_reset(void) { g_bb_alpha_seq = 0; }
/*======================================== ALPHA-LABEL FILL & DISPATCH MACROS ========================================*/
void bb_fill_alpha(IR_t *nd) {
    extern int g_m4_dense_nid;
    bb_label_t *a = &g_α_ring[g_α_ring_i++ & 7];
    if (g_m4_dense_nid) emit_label_initf(a, "bb%d_α", ++g_bb_alpha_seq);
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
static int is_pat_chain_elem(IR_e t);
static void emit_cat_diamond(IR_t *entry, IR_t *J, bb_label_t *fwd, bb_label_t *fail, bb_label_t *arm_beta, int id, int armidx);
/*--------------------------------------------------------------------------------------------------------------------*/
static int is_pat_chain_elem(IR_e t);
static int gather_lowered_cat_arms(IR_t *entry, IR_t **arms, IR_t **joins, int cap, IR_t **cat_out, IR_t *stop);
/* Collect γ/ω-threaded inline alt arms: arm[0]=start, arm[i+1]=arm[i]->ω.node while ω is a pat elem.
   Returns number of arms collected (1 = single arm, no alternation chain). */
/*--------------------------------------------------------------------------------------------------------------------*/
static void gz_emit_cell(IR_t *g, bb_label_t *next_γ, bb_label_t *gw, bb_label_t *gβ, bb_label_t *cut_ω, pl_gz_callee_vec_t *cv);
/* catch box: α marks the trail then runs the goal chain; goal.γ -> catch success (next_γ);
 * goal.ω -> the ball-check handler (op_sa 1): no pending throw -> ω (plain failure); pending ->
 * unwind to mark, gzu_build the catcher, rt_pl_throw_match -> recovery chain on match, else ω
 * leaving the ball set so an outer catch re-catches. Recovery chain: γ->next_γ, ω->catch.ω. */
static void gz_emit_catch(IR_t *g, bb_label_t *next_γ, bb_label_t *gw, bb_label_t *gβ, bb_label_t *cut_ω, pl_gz_callee_vec_t *cv) {
    fprintf(stderr, "GROUND ZERO: %s not implemented (Icon-only reset)\n", "gz_emit_catch"); abort(); }
/*--------------------------------------------------------------------------------------------------------------------*/
void resolve_choice_clause_label(char *dst, size_t dsz, int id, int ci, const char *suffix) {
    fprintf(stderr, "GROUND ZERO: %s not implemented (Icon-only reset)\n", "resolve_choice_clause_label"); abort(); }
/*--------------------------------------------------------------------------------------------------------------------*/
int bb_kind_is_driver_owned(int t) { fprintf(stderr, "GROUND ZERO: %s not implemented (Icon-only reset)\n", "bb_kind_is_driver_owned"); abort(); }
/*--------------------------------------------------------------------------------------------------------------------*/
void bb_prepare(IR_t *nd) {
    if (!PLATFORM_X86) return;
    g_emit.bb_ls = NULL;
    g_emit.bb_rs = NULL;
    g_emit.bb_op_lbl = NULL;
    g_emit.bb_lk = -1;
    (void) nd;
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*======================================== IR OPERAND / ARITHMETIC PREDICATES ========================================*/
static int bb_arith_dyn_kind(IR_t *o) { return o && (o->op == IR_CALL || ir_is_call_kind(o->op)); }
static int bb_arith_materializable(IR_t *o) {
    return o && (o->op == IR_CALL || ir_is_call_kind(o->op) || o->op == IR_LIT_INTEGER || (o->op == IR_VAR && IR_LIT(o).sval));
}
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
/*--------------------------------------------------------------------------------------------------------------------*/
int arith_emits_descr(IR_t *o) {
    if (!o || o->op != IR_BINOP || !bb_arith_is_dynamic(o)) return 0;
    IR_t *a = bb_child0(o), *b = bb_child1(o);
    int a_ok = a && (a->op == IR_CALL || ir_is_call_kind(a->op) || arith_emits_descr(a));
    int b_ok = b && (b->op == IR_CALL || ir_is_call_kind(b->op) || arith_emits_descr(b));
    return a_ok && b_ok;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int binop_slot_kind(IR_t *nd) {
    int64_t op = nd ? IR_LIT(nd).ival : -1;
    if ((op >= BINOP_LT && op <= BINOP_NE) || (op >= BINOP_SLT && op <= BINOP_SNE)) return BINOP_CAT_RELOP;
    if (op == BINOP_CONCAT)               return BINOP_CAT_CONCAT;
    if (op == BINOP_ADD || op == BINOP_SUB || op == BINOP_MUL || op == BINOP_DIV || op == BINOP_MOD || op == BINOP_POW) return BINOP_CAT_ARITH;
    if (op == BINOP_CUNION || op == BINOP_CDIFF || op == BINOP_CINTER) return BINOP_CAT_ARITH;
    return BINOP_CAT_ARITH;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int descr_binop_opnd_slot(IR_t *o) {
    /* TMP DOCTRINE: the drive-order slotmap is a memo, not the truth — LOWER's grant (o->tmp) is. A consumer BFS-ordered before its operand's drive (ipxref: BINOP before CALL_BUILTIN) missed the
       memo and aborted despite a valid grant; drive_value_slot registers off==tmp for every converted node, so the fallback is the same offset, order-independent. */
    if (!o || o->op == IR_LIT_REAL) return -1;
    int s = bb_slot_get(o); if (s >= 0) return s;
    return (o->tmp >= 0) ? o->tmp : -1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int binop_operand_real_static(IR_graph_t *g, IR_t *o, int depth);
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
int binop_is_num_real(IR_graph_t *g, IR_t *nd) {
    if (!nd) return 0;
    int64_t op = IR_LIT(nd).ival;
    if (op == BINOP_POW) return 1;
    if (op == BINOP_CUNION || op == BINOP_CDIFF || op == BINOP_CINTER) return 1;
    int is_num = (op == BINOP_ADD || op == BINOP_SUB || op == BINOP_MUL || op == BINOP_DIV || op == BINOP_MOD || (op >= BINOP_LT && op <= BINOP_NE));
    if (!is_num) return 0;
    return binop_operand_real_static(g, bb_child0(nd), 0) || binop_operand_real_static(g, bb_child1(nd), 0);
}
/*--------------------------------------------------------------------------------------------------------------------*/
/* Emit e's value-operands (e.g. the from/to bounds of an IR_TO) so they get frame slots before e itself is walked.
   For a single-node value producer (literal, var) e has no operands and this is a no-op.  Mirrors case_slot_binop_operands. */
/*--------------------------------------------------------------------------------------------------------------------*/
/* IR_REPALT — Icon repeated alternation `|e`.  The sub-expression e is the lowerer-set operand[0]; its chain nodes are
   BFS-discovered and emitted in the main loop, but flat_drive_repalt owns all four of e's EDGES (e_root's γ/ω are
   redirected to the yield/test stubs below at edge-resolution time).  Modelled on JCON ir_a_RepAlt:
   a one-bit `yielded` flag (frame slot at off+16, like bb_limit's counter) toggles JCON's MoveLabel/IndirectGoto.
     restart (= REPALT α / fresh start): yielded:=0; (re-)evaluate e's bounds; run e.
     e succeeds:  copy e's value into the REPALT slot; yielded:=1; jmp γ (yield).
     e fails:     if yielded, jmp restart (re-run e from scratch — the infinite repeat); else jmp ω (|e produced nothing).
     REPALT β (consumer resume): jmp e-β (pump e for its next value, no bounds re-eval).
   This yields e's value-sequence over and over, but fails immediately if a fresh start of e produces nothing. */
/*--------------------------------------------------------------------------------------------------------------------*/
static int to_inner_gen_operand_k(IR_t *gi, IR_t **nodes, int n) {
    int bk = -1;
    if (gi->op != IR_TO && gi->op != IR_TO_BY) return -1;
    for (int oi = 0; oi < gi->n_operands; oi++) for (int k = 0; k < n; k++) if (nodes[k] == (IR_t *)gi->operands[oi] && ir_is_generator_kind(nodes[k]->op) && k > bk) bk = k;
    return bk;
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*=========================================== CALL ROUTING CLASSIFICATION ============================================*/
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
/*--------------------------------------------------------------------------------------------------------------------*/
int bb_call_route_classify(IR_t * nd) {
    const char * fn = g_emit.op_sval ? g_emit.op_sval : ""; int64_t narg = g_emit.op_ival; IR_t * a0 = ir_call_arg(nd, 0); double dv = g_emit.op_dval;
    IR_e k = nd ? nd->op : IR_CALL;
    /* op-field call-kind (resolved at pre-emit time from the proc/builtin name tables, no dval tag) wins first */
    if (k == IR_CALL_BUILTIN && fn[0] && rt_builtin_is_generator(fn)) return CALL_ROUTE_BYNAME;
    if (k == IR_CALL_PROC_STAGED) return CALL_ROUTE_PROC_STAGED;
    if (g_gvar_flat_chain && k == IR_CALL_GVAR_USERPROC) return CALL_ROUTE_GVAR_USERPROC;
    if (k == IR_CALL_BUILTIN && g_emit.op_write_route == 0 && fn[0] && rt_builtin_is_known(fn)) return CALL_ROUTE_FN;
    if (dv == 2.0 && fn[0] && rt_builtin_is_known(fn)) return CALL_ROUTE_BYNAME;
    if (dv == 2.0 && !strcmp(fn, "__rk_bool")) return CALL_ROUTE_RK_BOOL_COND;
    /* A user-defined generator proc that happens to share a builtin name (e.g. `upto`) must route as a
       proc generator, not the builtin — registered+generator wins over rt_builtin_is_generator below. */
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
/*--------------------------------------------------------------------------------------------------------------------*/
void walk_bb_flat(IR_t *nd, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) { fprintf(stderr, "GROUND ZERO: %s not implemented (Icon-only reset)\n", "walk_bb_flat"); abort(); }
/*--------------------------------------------------------------------------------------------------------------------*/
/*===================== TEMPLATE DISPATCH  -  walk_bb_node (the one per-node template selector) ======================*/
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
    g_emit.op_sval = (nd->op == IR_VAR || nd->op == IR_VAR_REF || nd->op == IR_ASSIGN || nd->op == IR_LIT_STRING || nd->op == IR_LIT_CHARSET || nd->op == IR_KEYWORD || nd->op == IR_FIELD_GET || nd->op == IR_FIELD_VAR || nd->op == IR_FIELD_SET || nd->op == IR_SUBSCRIPT || nd->op == IR_ITERATE || nd->op == IR_NULLTEST_VAR || nd->op == IR_PROC_GEN || nd->op == IR_PROC_VALUE || ir_norm_call_kind(nd->op) == IR_CALL) ? IR_LIT(nd).sval : (const char *)0;
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
    case IR_KEYWORD:              bb_emit_x86(bb_keyword());            return 0;
    case IR_VAR:                  { extern int is_global(const char *);
        if (IR_LIT(nd).sval && IR_LIT(nd).sval[0] == '&') bb_emit_x86(bb_keyword());
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
    case IR_REPALT:               /* driven by flat_drive_repalt — no direct template call here */ return 0;
    case IR_ITERATE:              bb_emit_x86(bb_iterate(nd));      return 0;
    case IR_SCAN_ENTER:           { g_emit.op_sb = 1; g_emit.op_sa = g_emit.op_a_slot; bb_emit_x86(bb_gen_scan()); } return 0;
    case IR_INITIAL:           bb_emit_x86(bb_enter_init());     return 0;
    case IR_CREATE:                bb_emit_x86(bb_create());        return 0;
    case IR_ACTIVATE:              bb_emit_x86(bb_activate());      return 0;
    case IR_CORET:                 bb_emit_x86(bb_coret());         return 0;
    case IR_COFAIL:                bb_emit_x86(bb_cofail());        return 0;
    case IR_MOVE_LABEL:            bb_emit_x86(bb_move_label());    return 0;
    case IR_INDIRECT_GOTO:         bb_emit_x86(bb_indirect_goto()); return 0;
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
    case IR_CALL_PROC_STAGED: case IR_CALL_USERPROC: case IR_CALL_BYNAME: case IR_CALL_BUILTIN: case IR_CALL_GVAR_USERPROC:
    case IR_PROC_GEN:
    case IR_CALL: {
        bb_emit_x86(bb_call(nd));
        return 0;
    }
    case IR_FAIL:            bb_emit_x86(bb_fail());                            return 0;
    case IR_UNOP:
    case IR_UNOP_TEST: case IR_NULLTEST_VAR: bb_emit_x86(bb_unop());   return 0;
    case IR_FIELD_GET: case IR_FIELD_VAR: bb_emit_x86(bb_field_get());   return 0;
    case IR_FIELD_SET:            bb_emit_x86(bb_field_set());      return 0;
    default:
        fprintf(out, "# [walk_bb_node: kind=%d unhandled]\n", (int)nd->op);
        return 1;
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*=============== THE ONE EMITTER DRIVER  -  emit_drive (reads IR, owns slots/edges, never mutates IR) ===============*/
extern int           bb_slot_get(IR_t *nd);
extern void          bb_slot_register(IR_t *nd, int off);
extern int           bb_varslot_peek(const char *name);
extern int           is_global(const char *name);
extern int           gva_index_of(const char *name);
extern int           g_gva_active;
extern IR_graph_t *  g_emit_cfg;
/*====================================================================================================================*/
#define DRIVE_FILL(nd,s,f,b) do { \
    bb_fill_alpha(nd); \
    g_emit.lbl_γ=(s)->name; g_emit.lbl_ω=(f)->name; g_emit.lbl_β=(b)->name; \
    g_emit.lbl_γ_p=(s); g_emit.lbl_ω_p=(f); g_emit.lbl_β_p=(b); \
    walk_bb_node((nd), emit_outf()); } while(0)
#define DRIVE_PAIR_RESET()      do { g_emit.xa_bb_emit_pair_n = 0; } while(0)
#define DRIVE_PAIR_JMP(tgt)     do { int _i=g_emit.xa_bb_emit_pair_n++; g_emit.xa_bb_emit_pair_define[_i]=NULL; g_emit.xa_bb_emit_pair_jmp[_i]=(tgt); } while(0)
#define DRIVE_PAIR_DEF_JMP(l,t) do { int _i=g_emit.xa_bb_emit_pair_n++; g_emit.xa_bb_emit_pair_define[_i]=(l); g_emit.xa_bb_emit_pair_jmp[_i]=(t); } while(0)
/*--------------------------------------------------------------------------------------------------------------------*/
static int drive_value_slot(IR_t *nd) {
    int e = bb_slot_get(nd);
    if (e >= 0) return e;
    if (nd && nd->tmp >= 0) { bb_slot_register(nd, nd->tmp); return nd->tmp; }
    if (nd && ir_node_produces_value(nd->op)) {
        fprintf(stderr, "FATAL drive_value_slot: IR op=%d is a value-producer with no nd->tmp — ir_drive_slot_assign never granted it. Emit-time allocation is ERADICATED (TMP-ERADICATE); add the grant in LOWER, never patch it here. op=%d\n", (int)nd->op, (int)nd->op);
        abort();
    }
    fprintf(stderr, "[TE] GOUGE drive_value_slot(op=%d): non-value-producer asked for a slot with no LOWER grant — emit-time allocation is ERADICATED (TMP-ERADICATE); add an ir_drive_slot_assign grant for this op\n", nd ? (int)nd->op : -1);
    abort();
}
/*--------------------------------------------------------------------------------------------------------------------*/
/* flat_drive_repalt — the function emit.cpp's REPALT comments cite as owning e's four edges (previously a phantom: cited, never defined — 2026-07-01 wholesale audit).  Layout per the IR_REPALT design
   note above + JCON ir_a_RepAlt (refs/jcon-master/tran/irgen.icn:206-229): α(lbls[i]) clear→jmp-e ≡ MoveLabel(t,fail);Goto e.start · yield(ra_y) copy+flag:=1→jmp-γ ≡ MoveLabel(t,start);Goto success ·
   test(ra_t) flag==1?je-α:jmp-ω ≡ IndirectGoto(t) · β jmp e-β when e_root is generator_kind else jmp-test (a non-generator's resume IS its failure — the ml ival=0 rule).  e_root's γ/ω edges are
   redirected onto ra_y/ra_t by the edge-resolution scan in the main loop; this function only emits the four stubs. */
/*--------------------------------------------------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------------------------------------------------*/
static void drive_arg_slots_reserve(int n) {
    if (n <= g_emit.op_arg_slot_cap) return;
    int cap = g_emit.op_arg_slot_cap > 0 ? g_emit.op_arg_slot_cap : 16;
    while (cap < n) cap *= 2;
    int *grown = (int *)realloc(g_emit.op_arg_slot, (size_t)cap * sizeof(int));
    if (!grown) { fprintf(stderr, "FATAL emit_drive: arg-slot staging realloc to %d failed\n", cap); abort(); }
    g_emit.op_arg_slot = grown; g_emit.op_arg_slot_cap = cap;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void drive_unowned(IR_t *nd) {
    fprintf(stderr, "FATAL emit_drive: IR op=%d has no template in the universal driver. Every op must be handled; the driver never declines silently. Implement op=%d. NOTE: this is also the guard sink inside existing cases — if op=N plainly has a case, the BACKTRACE LINE (not this message) names the failing guard.\n",
            nd ? (int)nd->op : -1, nd ? (int)nd->op : -1);
    abort();
}
/*--------------------------------------------------------------------------------------------------------------------*/
void emit_drive(IR_t *nd, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    if (!nd) { drive_unowned(nd); return; }
    switch (nd->op) {
    case IR_LIT_STRING: case IR_LIT_INTEGER: case IR_LIT_REAL:
        g_emit.op_off = drive_value_slot(nd); DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_LIT_CHARSET:
        /* bb_lit_scalar's CHARSET arm keys on op_node_kind and emits the slen=-1 IS_CSET_fn sentinel itself; walk's preamble owns op_ival (the old op_ival=1 staging here was dead — clobber audit 2026-07-02) */
        g_emit.op_sval = IR_LIT(nd).sval; g_emit.op_off = drive_value_slot(nd); DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_KEYWORD:
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
        if (!g_emit.op_num_real) { sa = descr_binop_opnd_slot(bb_child0(nd)); sb = descr_binop_opnd_slot(bb_child1(nd)); }
        /* LOOP-CARRIED LOCAL-VAR READ FIX (loop-carried-read class, every+augop family, 2026-07-01):
           a bare-VAR operand's producer slot (bb_slot_get) is written ONCE at that VAR node's own α and
           never revisited on generator resume (the chain-BFS routes resume straight at the generator,
           per DIVISION RULE), so a chain like `sum+:=gen` reads a frozen pre-loop snapshot every pass
           even though IR_ASSIGN keeps sum's real varslot current. JCON's ir_a_Binop/ir_augmented_assignment
           never hit this because an Ident operand is re-read at the point of use, not cached from a
           one-time evaluation. bb_varslot_peek(name) IS that live point-of-use read for SCRIP locals (the
           same persistent name-keyed cell IR_ASSIGN's own driver arm writes via bb_varslot) — reading it
           here instead of the VAR node's copy-once slot costs nothing in the non-generator case (identical
           value, nothing else runs between the VAR node's copy and this read) and fixes the generator case.
           Globals need their own NV_GET-fresh-read arm (not this table lookup) — deliberately out of scope
           here; falls through to the pre-existing (buggy for this one shape) path. See GOAL-IR-IMMUTABLE-EMIT.md
           watermark for the full trace this fix is based on. */
        { IR_t *c0 = bb_child0(nd);
          if (c0 && c0->op == IR_VAR && IR_LIT(c0).sval && IR_LIT(c0).sval[0] != '&' && !is_global(IR_LIT(c0).sval)) {
              int voff = bb_varslot_peek(IR_LIT(c0).sval); if (voff >= 0) sa = voff; }
          IR_t *c1 = bb_child1(nd);
          if (c1 && c1->op == IR_VAR && IR_LIT(c1).sval && IR_LIT(c1).sval[0] != '&' && !is_global(IR_LIT(c1).sval)) {
              int voff = bb_varslot_peek(IR_LIT(c1).sval); if (voff >= 0) sb = voff; }
        }
        if (sa < 0 || sb < 0) { drive_unowned(nd); break; }
        g_emit.op_sa = sa; g_emit.op_sb = sb; g_emit.op_off = drive_value_slot(nd); g_emit.op_binop_kind = (int)binop_slot_kind(nd);
        DRIVE_PAIR_RESET(); DRIVE_PAIR_DEF_JMP(lbl_β, lbl_ω); DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_UNOP: case IR_UNOP_TEST: case IR_NULLTEST_VAR: {
        int sa = descr_binop_opnd_slot(bb_child0(nd));
        if (sa < 0) { drive_unowned(nd); break; }
        g_emit.op_sa = sa; g_emit.op_off = drive_value_slot(nd); DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_FIELD_GET: case IR_FIELD_VAR: {
        IR_t * obj = bb_child0(nd);
        int sa = obj ? descr_binop_opnd_slot(obj) : -1;
        if (sa < 0) { drive_unowned(nd); break; }
        g_emit.op_a_slot = sa; g_emit.op_sval = IR_LIT(nd).sval; g_emit.op_off = drive_value_slot(nd);
        DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_FIELD_SET: {
        IR_t * obj = bb_child0(nd); IR_t * val = bb_child1(nd);
        int sa = obj ? bb_slot_get(obj) : -1; int sb = val ? bb_slot_get(val) : -1;
        if (sa < 0 || sb < 0) { drive_unowned(nd); break; }
        g_emit.op_a_slot = sa; g_emit.op_sb = sb; g_emit.op_sval = IR_LIT(nd).sval;
        DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_ASSIGN: {
        const char *vn = IR_LIT(nd).sval;
        if (!vn) { fprintf(stderr, "FATAL emit_drive IR_ASSIGN guard: nameless %d-operand assign (assign-through-lvalue-producer: !x/?x element-variable or s[i:j] section) — LOWER's TT_ASSIGN terminal arm minted a placeholder; not a missing template. ASSIGN-LV rung, GOAL-IR-IMMUTABLE-EMIT.md.\n", nd->n_operands); abort(); }
        if (is_global(vn)) {
            g_emit.op_sb = -1; g_emit.op_sval = vn; g_emit.op_gva_k = g_gva_active ? gva_index_of(vn) : -1;
            g_emit.op_off = drive_value_slot(nd); DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
        }
        { int voff = bb_varslot_peek(vn);
          if (voff < 0) { fprintf(stderr, "[TE-4] IR_ASSIGN local '%s' has no LOWER-granted varslot — grant it in ir_drive_slot_assign (scrip_ir.c), never allocate in the emitter\n", vn); abort(); }
          g_emit.op_sb = voff; }
        g_emit.op_off = drive_value_slot(nd); DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_CALL: case IR_CALL_BUILTIN: case IR_CALL_PROC_STAGED: case IR_CALL_USERPROC: case IR_CALL_BYNAME: case IR_CALL_GVAR_USERPROC: case IR_PROC_GEN: {
        int na = nd->n_operands; drive_arg_slots_reserve(na);
        for (int i = 0; i < na; i++) { IR_t * a = ir_call_arg(nd, i); g_emit.op_arg_slot[i] = (a && a->tmp >= 0) ? a->tmp : -1; }
        g_emit.op_arg_slot_n = na; g_emit.op_write_route = bb_call_write_route(nd);
        DRIVE_PAIR_RESET(); DRIVE_PAIR_DEF_JMP(lbl_β, lbl_ω); DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_TO: {
        if (!bb_child0(nd) || !bb_child1(nd)) { drive_unowned(nd); break; }
        g_emit.op_sa = drive_value_slot(bb_child0(nd)); g_emit.op_sb = drive_value_slot(bb_child1(nd));   /* tmp-doctrine: order-independent */
        g_emit.op_sc = -1;   /* IR_TO = 2-operand form, implicit by=1 (TO-SPLIT); by lives on IR_TO_BY */
        g_emit.op_num_real = (IR_LIT(nd).sval && strcmp(IR_LIT(nd).sval, "ar") == 0) ? 1 : 0;
        g_emit.op_off = drive_value_slot(nd);
        DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_TO_BY: {
        if (!bb_child0(nd) || !bb_child1(nd) || nd->n_operands < 3 || !nd->operands[2]) { drive_unowned(nd); break; }
        g_emit.op_sa = drive_value_slot(bb_child0(nd)); g_emit.op_sb = drive_value_slot(bb_child1(nd));   /* tmp-doctrine: order-independent */
        g_emit.op_sc = drive_value_slot(nd->operands[2]);   /* by producer's slot; sign checked at runtime in bb_to_by */
        g_emit.op_num_real = 0;
        g_emit.op_off = drive_value_slot(nd);
        DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_MAKE_LIST: {
        int na = nd->n_operands; drive_arg_slots_reserve(na);
        for (int i = 0; i < na; i++) { IR_t * a = nd->operands[i]; g_emit.op_arg_slot[i] = a ? drive_value_slot(a) : -1; }
        g_emit.op_arg_slot_n = na;
        g_emit.op_off = drive_value_slot(nd);   /* result DESCR at tmp; argv scratch at tmp+16 (slot-grant, scrip_ir.c) */
        DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_REV_ASSIGN: {
        /* x <- v (reversible assign). operands[0]=rhs producer, [1]=lhs IR_VAR name carrier.
           op_sb = destination varslot; op_off = own value slot; op_sc = save area (off+16, inside the
           k+=2 grant); rhs source = op_a_slot, which walk_bb_node's preamble re-derives from operands[0]
           AFTER this case runs (clobber pattern — the operand order in LOWER exists to make that
           re-derivation correct; do not stage op_a_slot here, walk owns it). */
        IR_t * lv = nd->n_operands > 1 ? nd->operands[1] : NULL;
        const char * vn = lv ? IR_LIT(lv).sval : NULL;
        if (!vn || nd->n_operands < 1 || !nd->operands[0]) { drive_unowned(nd); break; }
        { int voff = bb_varslot_peek(vn);
          if (voff < 0) { fprintf(stderr, "[TE-4] IR_REV_ASSIGN local '%s' has no LOWER-granted varslot — grant it in ir_drive_slot_assign (scrip_ir.c), never allocate in the emitter\n", vn); abort(); }
          g_emit.op_sb = voff; }
        g_emit.op_off = drive_value_slot(nd);
        g_emit.op_sc  = g_emit.op_off + 16;
        DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_REV_ASSIGN_VAR: {
        /* x[i] <- v (reversible assign through an operand-carried variable — the IDX-UNIFY sibling of
           IR_REV_ASSIGN, IR_ASSIGN_VAR's operand order). operands[0]=variable producer (walk_bb_node's
           preamble re-derives op_a_slot from it — do not stage op_a_slot here, walk owns it; the
           drive_value_slot call below REGISTERS the slot the preamble re-derives), [1]=rhs value.
           op_sa = value slot; op_off = own value slot; op_sc = save area (off+16, inside the k+=2 grant). */
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
        /* If the SEQ_EXPR join has its own tmp slot AND an operand with a slot (value-forward pattern),
           emit a 2×8 copy from the operand slot to the join's slot so the consumer reads the join's slot.
           When the join slot == operand slot (inherited), no copy needed. */
        if (nd->tmp >= 0 && nd->n_operands > 0 && nd->operands[0]) {
            int src = bb_slot_get(nd->operands[0]);
            int dst = nd->tmp;
            if (src >= 0 && dst >= 0 && src != dst) {
                /* This SEQ_EXPR join forwards a value: copy operand value → its own slot */
                /* We emit this as a text/binary inline via the SEQ_EXPR's emit rather than a template */
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
        g_emit.lbl_t1_p = lbl_β;   /* this suspend's own β — needed by bb_suspend to store resume-ptr */
        g_emit.op_sb    = g_suspend_resume_slot;
        DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_SUBSCRIPT: {
        /* IR_SUBSCRIPT = s[i:j] / s[i+:n] / s[i-:n].  operands[0]=base, [1]=i1, [2]=i2.
           op_a_slot = base DESCR slot; op_sa = i1 slot; op_sb = i2 slot; op_off = result slot.
           op_ival = section variant (0=plain, 1=plus, 2=minus). */
        if (nd->n_operands == 2) {
            /* IDX-UNIFY 2-operand form: x[i] variable producer. operands[0]=base, [1]=index; op_a_slot=base, op_sa=index, op_off=result (DT_N-trap variable or pass-through value). */
            IR_t * vb = nd->operands[0]; IR_t * vi = nd->operands[1];
            int va = vb ? drive_value_slot(vb) : -1; int vs = vi ? drive_value_slot(vi) : -1;
            if (va < 0 || vs < 0) { drive_unowned(nd); break; }
            g_emit.op_a_slot = va; g_emit.op_sa = vs; g_emit.op_off = drive_value_slot(nd);
            DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
        }
        IR_t * base = nd->n_operands > 0 ? nd->operands[0] : NULL;
        IR_t * i1   = nd->n_operands > 1 ? nd->operands[1] : NULL;
        IR_t * i2   = nd->n_operands > 2 ? nd->operands[2] : NULL;
        int sa = base ? drive_value_slot(base) : -1;   /* tmp doctrine: order-independent (0f4f105b fix class) */
        int sb = i1   ? drive_value_slot(i1)   : -1;
        int sc = i2   ? drive_value_slot(i2)   : -1;
        if (sa < 0 || sb < 0 || sc < 0) { drive_unowned(nd); break; }
        g_emit.op_a_slot = sa; g_emit.op_sa = sb; g_emit.op_sb = sc;
        g_emit.op_off = drive_value_slot(nd);
        DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_RANDOM: {
        /* ASSIGN-LV LV-3a: ?x — single-shot random-element producer (operator{0,1}); operands[0]=base. Rolls the canonical LCG once in rt_random_var; VCELL (list/record/table/string-var) or value (cset/string-value/?n/?0). */
        IR_t * v = nd->n_operands > 0 ? nd->operands[0] : NULL;
        int sa = v ? drive_value_slot(v) : -1;
        if (sa < 0) { drive_unowned(nd); break; }
        g_emit.op_a_slot = sa; g_emit.op_off = drive_value_slot(nd);
        DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_DEREF: {
        /* IDX-UNIFY: materialize the value of a variable (identity on non-trap). operands[0]=variable node. */
        IR_t * v = nd->n_operands > 0 ? nd->operands[0] : NULL;
        int sa = v ? drive_value_slot(v) : -1;
        if (sa < 0) { drive_unowned(nd); break; }
        g_emit.op_a_slot = sa; g_emit.op_off = drive_value_slot(nd);
        DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_ASSIGN_VAR: {
        /* IDX-UNIFY: assign through an operand-carried variable. operands[0]=variable, [1]=value; result = value. */
        IR_t * v = nd->n_operands > 0 ? nd->operands[0] : NULL;
        IR_t * r = nd->n_operands > 1 ? nd->operands[1] : NULL;
        int sa = v ? drive_value_slot(v) : -1; int sb = r ? drive_value_slot(r) : -1;
        if (sa < 0 || sb < 0) { drive_unowned(nd); break; }
        g_emit.op_a_slot = sa; g_emit.op_sa = sb; g_emit.op_off = drive_value_slot(nd);
        DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_SWAP: {
        /* IR_SWAP = x:=:y.  operands[0]=x-var, [1]=y-var.  Both must be frame-slot variables. */
        IR_t * xv = nd->n_operands > 0 ? nd->operands[0] : NULL;
        IR_t * yv = nd->n_operands > 1 ? nd->operands[1] : NULL;
        int sa = xv ? bb_varslot_peek(IR_LIT(xv).sval) : -1;
        int sb = yv ? bb_varslot_peek(IR_LIT(yv).sval) : -1;
        if (sa < 0 || sb < 0) { drive_unowned(nd); break; }
        g_emit.op_sa = sa; g_emit.op_sb = sb; g_emit.op_off = drive_value_slot(nd);
        DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_PROC_VALUE: {
        /* PROC-VALUE leaf: first-class procedure value for a baked name (walk preamble whitelists op_sval). */
        g_emit.op_off = drive_value_slot(nd);
        DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_CALL_VALUE: {
        /* PROC-VALUE by-value invoke: operands[0]=callee value producer, [1..]=args. Callee slot in op_sa,
           arg slots in op_arg_slot[] (the MAKE_LIST idiom), argv scratch at tmp+16 (call-family 1+n grant). */
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
        /* SWAP-LV: x :=: y through operand-carried variables (canonical swap, oasgn.r:265 — underef both, two
           GeneralAsgn with same-string tvsubs pos adjustment inside rt_swap_var). operands[0]=x-var, [1]=y-var;
           result = fresh deref of x after both writes. op_a_slot re-derived equal by walk's preamble; op_sa/op_off survive. */
        IR_t * xv = nd->n_operands > 0 ? nd->operands[0] : NULL;
        IR_t * yv = nd->n_operands > 1 ? nd->operands[1] : NULL;
        int sa = xv ? drive_value_slot(xv) : -1; int sb = yv ? drive_value_slot(yv) : -1;
        if (sa < 0 || sb < 0) { drive_unowned(nd); break; }
        g_emit.op_a_slot = sa; g_emit.op_sa = sb; g_emit.op_off = drive_value_slot(nd);
        DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_LIMIT: {
        /* IR_LIMIT = e \ n.  operands[0]=generator node, operands[1]=count expr (an on-spine value-producer).
           op_sa = generator value slot (what LIMIT copies to consumer);
           op_sc = count value slot (runtime read — literal and variable counts ride the same slot path; the old
           op_ival immediate staging was DEAD, discarded by walk's preamble re-derivation — clobber audit 2026-07-02);
           lbl_t0 = generator β (for β-pump on resume). op_off = LIMIT own slot (result copy at off, counter at off+16). */
        IR_t * gen = nd->n_operands > 0 ? nd->operands[0] : NULL;
        IR_t * cnt = nd->n_operands > 1 ? nd->operands[1] : NULL;
        int sa = gen ? drive_value_slot(gen) : -1;   /* tmp-doctrine: slot = gen->tmp even if gen is later in chain order */
        int sc = cnt ? drive_value_slot(cnt) : -1;
        if (sa < 0 || sc < 0) { drive_unowned(nd); break; }
        g_emit.op_sa = sa; g_emit.op_sc = sc;
        /* g_limit_gen_beta is set by the BFS intercept above before emit_drive is called */
        g_emit.lbl_t0   = g_limit_gen_beta ? g_limit_gen_beta->name : (lbl_β ? lbl_β->name : NULL);
        g_emit.lbl_t0_p = g_limit_gen_beta ? g_limit_gen_beta : lbl_β;
        g_emit.op_off = drive_value_slot(nd);
        DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_CREATE: {
        /* IR_CREATE = `create EXPR` (RUNG 3, Claude Sonnet 2026-07-01).  operand[0]=body's entry node.
           create ALWAYS succeeds immediately (RUNG 1: "create EXPR itself SUCCEEDS IMMEDIATELY") —
           bb_create.cpp's α is unconditional success; there is no failure path from create itself
           (matches JCON's ir_a_Create: `Goto p.ir.success` unconditionally, no failure edge at all).
           op_off = this create-site's OWN slot (holds a heap-allocated scrip_coctx_t* — see
           bb_create.cpp; 16 bytes granted, only 8 used, same stride as every other value-producer).
           op_sval_lbl = the body's entry label STRING, resolved by the BFS pre-pass above (the
           nodes[i]->op == IR_CREATE block, same file, ~80 lines up) — NOT recomputed here, since by
           the time emit_drive runs for THIS node the pre-pass has already run for every node in this
           chain (codegen_flat_chain_body is two-phase: full BFS discovery + label allocation BEFORE
           any node is driven — see that function's own structure). If op_sval_lbl is NULL here, the
           pre-pass's scan didn't find operand[0] in nodes[] — shouldn't happen per RUNG 1's own
           lowering, but bb_create.cpp's own guard bombs loudly on NULL rather than emit a bad LEA. */
        g_emit.op_off = drive_value_slot(nd);
        g_emit.lbl_t0   = g_create_body_entry ? g_create_body_entry->name : NULL;
        g_emit.lbl_t0_p = g_create_body_entry;
        DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_ACTIVATE: {
        /* IR_ACTIVATE (RUNG 5) — `e1 @ e2` / `@e`. operand[0] = the coexpression value node (its slot holds
           the scrip_coctx_t* bb_create stored at DESCR d0); operand[1] = the transmitted value node, ABSENT
           for the unary form (op_sb = -1 → bb_activate transmits &null {DT_SNUL,0} inline). op_off = this
           node's own 16-byte DESCR slot; bb_activate passes its ADDRESS as scrip_coexpr_activate's out-param
           so the coret'd value lands directly in the tmp with no post-copy. Read-only on nd (FACT RULE). */
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
        /* IR_CORET (RUNG 4) — the body of a `create` yields a value and hands control to the activator.
           operand[0] = the produced value node; op_sa = its 16-byte DESCR slot (bb_coret loads {rdi,rsi}
           from it and passes them to scrip_coret). IR_CORET produces no value of its OWN (excluded from
           ir_node_produces_value — it is a body-internal control terminal, not a general value-producer).
           γ/ω are NULL from RUNG 1's lowering and resolve to the chain defaults here; that post-yield
           continuation is a placeholder refined by RUNG 5's resume wiring (see bb_coret.cpp). */
        IR_t * pv = nd->n_operands > 0 ? nd->operands[0] : NULL;
        int sa = pv ? bb_slot_get(pv) : -1;
        if (sa < 0) { drive_unowned(nd); break; }
        g_emit.op_sa = sa;
        DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_COFAIL: {
        /* IR_COFAIL (RUNG 4) — the body of a `create` exhausted; mark the coexpression dead and hand control
           back to the activator. No operands, no value slot; bb_cofail calls scrip_cofail then jmp ω (which
           is unreachable, since scrip_cofail never returns). Emit-complete-but-unexercised until RUNG 5. */
        DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_MOVE_LABEL: {
        /* IR_MOVE_LABEL (unbounded alternation, JCON ir_MoveLabel + the shared-target value copy) — operands:
           [0]=the arm's resume-point node (resolved to a chain label by the pre-drive block into
           g_move_label_tgt, β vs α per this node's ival — the varying-α/β-per-circumstance case), [1]=the
           sibling IR_INDIRECT_GOTO (owner of the shared 32-byte cell: value DESCR at +0, label variable t at
           +16), [2]=the arm's own value node (op_sa; may be slotless for pure-control arms — template skips
           the copy). op_off = the shared cell; reserve +24 so the frame prologue covers the label word. */
        IR_t * ig = nd->n_operands > 1 ? nd->operands[1] : NULL;
        IR_t * av = nd->n_operands > 2 ? nd->operands[2] : NULL;
        int off = ig ? drive_value_slot(ig) : -1;
        g_emit.op_off = off;
        g_emit.op_sa = av ? bb_slot_get(av) : -1;
        g_emit.lbl_t0 = g_move_label_tgt ? g_move_label_tgt->name : NULL;
        g_emit.lbl_t0_p = g_move_label_tgt;
        DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_INDIRECT_GOTO: {
        /* IR_INDIRECT_GOTO (unbounded alternation, JCON ir_IndirectGoto at the alt's resume) — owns the shared
           cell (tmp, coordinated k+=2 in ir_drive_slot_assign): value DESCR at [tmp+0..15] (what lower_alt's
           *res hands every consumer), label variable t at [tmp+16..23] (what this node's α jumps through).
           No operands consumed here; every MoveLabel sibling writes both halves before this can execute. */
        int off = drive_value_slot(nd);
        g_emit.op_off = off;
        DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_ITERATE: {
        /* IR_ITERATE = !e  (list element generator).  operands[0]=list-value node.
           op_sa = list DESCR slot; op_sb = counter slot (at op_off+16); op_off = result slot. */
        IR_t * obj = nd->n_operands > 0 ? nd->operands[0] : NULL;
        int sa = obj ? bb_slot_get(obj) : -1;
        if (sa < 0) { drive_unowned(nd); break; }
        g_emit.op_sa = sa;
        g_emit.op_off = drive_value_slot(nd);
        g_emit.op_sb  = g_emit.op_off + 16;   /* counter slot adjacent to result */
        DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_INITIAL: {
        /* IR_INITIAL: JCON ir_EnterInit analog — "run this block only on the first call."
           nd->tmp is assigned by ir_drive_slot_assign (k+=1, 16 bytes).
           op_off = nd->tmp: [r12+op_off+8] holds the int64 done-flag (0=not run, 1=ran).
           γ = body entry (first call path), ω = skip-body path (subsequent calls + body exit). */
        g_emit.op_off = nd->tmp;
        if (g_emit.op_off < 0) { drive_unowned(nd); break; }
        DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_SCAN_ENTER: {
        /* IR_SCAN_ENTER: subject-string DESCR → rt_scan_enter → loads r13/r14/r15.
           operand[0] = subject node; op_sa = subject DESCR slot; op_sb = 1 (enter flag for bb_gen_scan).
           Saves old regs into frame slots nd->tmp..nd->tmp+23 (3×8 bytes).
           nd->tmp is assigned by ir_drive_slot_assign (k+=2 = 32 bytes, covers 24-byte need). */
        IR_t * subj = nd->n_operands > 0 ? nd->operands[0] : NULL;
        int sa = subj ? bb_slot_get(subj) : -1;
        if (sa < 0) { drive_unowned(nd); break; }
        g_emit.op_sa = sa; g_emit.op_sb = 1;
        g_emit.op_off = drive_value_slot(nd);   /* nd->tmp = save area for old r13/r14/r15 */
        { extern int g_scan_regs_live; g_scan_regs_live++; }
        DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_SCAN: {
        /* IR_SCAN (leave): restore r13/r14/r15 from frame slots.  op_sb = 0 (leave flag).
           operand[0] = the enter node; op_off = enter->tmp (the save-area slot).
           This replaces the old IR_LIT(nd).ival approach (which required knowing the slot at lower
           time, before slot assignment). */
        g_emit.op_sb  = 0;
        { extern int g_scan_regs_live; if (g_scan_regs_live > 0) g_scan_regs_live--; }
        IR_t * enter_nd = nd->n_operands > 0 ? nd->operands[0] : NULL;
        g_emit.op_off = (enter_nd && enter_nd->tmp >= 0) ? enter_nd->tmp : -1;
        if (g_emit.op_off < 0) { drive_unowned(nd); break; }
        DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_SCAN_TAB: {
        /* tab(n): op_off=own 24-byte slot. Delivery contract read by bb_scan_tab:
             op_sa >= 0 => runtime n in producer slot op_sa;
             op_sa <  0 => literal n = op_sb (ANY value; tab(0)/tab(-k) cvpos-normalized in the box).
           The literal branch owns op_sb for every integer value, so a slot-less runtime operand can
           no longer masquerade as literal 0 — it loud-aborts here instead of silently tabbing-to-end. */
        IR_t * a0 = nd->n_operands > 0 ? nd->operands[0] : NULL;
        g_emit.op_off = drive_value_slot(nd);
        if (!a0) { drive_unowned(nd); break; }
        if (a0->op == IR_LIT_INTEGER) { g_emit.op_sb = (int)IR_LIT(a0).ival; g_emit.op_sa = -1; }
        else { int sl = bb_slot_get(a0); if (sl < 0) { drive_unowned(nd); break; } g_emit.op_sa = sl; g_emit.op_sb = 0; }
        DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_SCAN_MOVE: {
        /* move(n): op_off=own 24-byte slot; op_sa=1 (flag); op_sb=literal n */
        IR_t * a0 = nd->n_operands > 0 ? nd->operands[0] : NULL;
        g_emit.op_off = drive_value_slot(nd);
        g_emit.op_sa = 1;
        g_emit.op_sb = (a0 && a0->op == IR_LIT_INTEGER) ? (int)IR_LIT(a0).ival : 1;
        DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_SCAN_POS: {
        /* pos(n): op_off=own 16-byte slot; op_sb=literal n */
        IR_t * a0 = nd->n_operands > 0 ? nd->operands[0] : NULL;
        g_emit.op_off = drive_value_slot(nd);
        g_emit.op_sb = (a0 && a0->op == IR_LIT_INTEGER) ? (int)IR_LIT(a0).ival : 1;
        DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_SCAN_UPTO: {
        /* upto(cset): op_off=own 24-byte slot; op_name1=literal cset C-string */
        IR_t * a0 = nd->n_operands > 0 ? nd->operands[0] : NULL;
        g_emit.op_off = drive_value_slot(nd);
        g_emit.op_name1 = (a0 && (a0->op == IR_LIT_STRING || a0->op == IR_LIT_CHARSET) && IR_LIT(a0).sval) ? IR_LIT(a0).sval : NULL;
        DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_SCAN_ANY: {
        /* any(cset): op_off=own 16-byte slot; op_name1=literal cset OR op_sa=var slot + op_name2 */
        IR_t * a0 = nd->n_operands > 0 ? nd->operands[0] : NULL;
        g_emit.op_off = drive_value_slot(nd);
        if (a0 && (a0->op == IR_LIT_STRING || a0->op == IR_LIT_CHARSET) && IR_LIT(a0).sval) {
            g_emit.op_name1 = IR_LIT(a0).sval; g_emit.op_sa = -1; g_emit.op_name2 = NULL;
        } else if (a0) {
            int sl = bb_slot_get(a0); g_emit.op_sa = sl; g_emit.op_name1 = NULL; g_emit.op_name2 = "?";
        } else { g_emit.op_name1 = NULL; g_emit.op_sa = -1; g_emit.op_name2 = NULL; }
        DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_SCAN_MANY: {
        /* many(cset): op_off=own 16-byte slot; op_name1=literal cset */
        IR_t * a0 = nd->n_operands > 0 ? nd->operands[0] : NULL;
        g_emit.op_off = drive_value_slot(nd);
        g_emit.op_name1 = (a0 && (a0->op == IR_LIT_STRING || a0->op == IR_LIT_CHARSET) && IR_LIT(a0).sval) ? IR_LIT(a0).sval : NULL;
        DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_SCAN_FIND: {
        /* find(needle): op_off=own 24-byte slot; op_name1=literal needle */
        IR_t * a0 = nd->n_operands > 0 ? nd->operands[0] : NULL;
        g_emit.op_off = drive_value_slot(nd);
        g_emit.op_name1 = (a0 && a0->op == IR_LIT_STRING && IR_LIT(a0).sval) ? IR_LIT(a0).sval : NULL;
        DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_SCAN_MATCH: {
        /* match(s): op_off=own 16-byte slot; op_name1=literal string */
        IR_t * a0 = nd->n_operands > 0 ? nd->operands[0] : NULL;
        g_emit.op_off = drive_value_slot(nd);
        g_emit.op_name1 = (a0 && a0->op == IR_LIT_STRING && IR_LIT(a0).sval) ? IR_LIT(a0).sval : NULL;
        DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_SCAN_BAL: {
        /* bal(cset): op_off=own 32-byte slot; op_name1=literal cset */
        IR_t * a0 = nd->n_operands > 0 ? nd->operands[0] : NULL;
        g_emit.op_off = drive_value_slot(nd);
        g_emit.op_name1 = (a0 && (a0->op == IR_LIT_STRING || a0->op == IR_LIT_CHARSET) && IR_LIT(a0).sval) ? IR_LIT(a0).sval : NULL;
        DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_REPALT: {
        /* IR_REPALT is NOT driven inline here — it is handled by flat_drive_repalt() which
           intercepts the node before emit_drive is called, drives the sub-expression e internally,
           and emits bb_repalt_{clear/yield/test} around e's chain. This arm should never fire in
           normal operation; if it does, it means flat_drive_repalt was not called. */
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
/*=============== FLAT CHAIN BODY  -  codegen_flat_chain_body (drives each chain node via emit_drive) ================*/
/*====================================================================================================================*/
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
        if ((c->op == IR_SUBSCRIPT || c->op == IR_RANDOM || c->op == IR_DEREF || c->op == IR_ASSIGN_VAR || c->op == IR_REV_ASSIGN_VAR || c->op == IR_SWAP_VAR || c->op == IR_CALL_VALUE) && c->ω.node && qt < CH_MAX) queue[qt++] = c->ω.node;   /* IDX-UNIFY: failure edges (je ω) must reach the walk — else/fail blocks were unemitted (rung16_subscript_sub_fail) */
        if (c->op == IR_SUSPEND && c->n_operands > 1 && c->operands[1] && qt < CH_MAX) queue[qt++] = c->operands[1];
        if (c->op == IR_CREATE && c->n_operands > 0 && c->operands[0] && qt < CH_MAX) queue[qt++] = c->operands[0];
        if (c->op == IR_MOVE_LABEL && c->n_operands > 0 && c->operands[0] && qt < CH_MAX) queue[qt++] = c->operands[0];
        if (c->op == IR_MOVE_LABEL && c->n_operands > 1 && c->operands[1] && qt < CH_MAX) queue[qt++] = c->operands[1];
        /* IR_REPALT operands ([0]=e root, [1]=e chain entry) — same latent hole the ml enqueue closed for
           alternation (2026-07-01 wholesale audit): without this the repeated expression is never BFS-
           discovered, the intercept's e_entry scan misses, e_lbl degrades to node_ω, REPALT fails instantly. */
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
        if ((c->op == IR_SUBSCRIPT || c->op == IR_RANDOM || c->op == IR_DEREF || c->op == IR_ASSIGN_VAR || c->op == IR_REV_ASSIGN_VAR || c->op == IR_SWAP_VAR || c->op == IR_CALL_VALUE) && c->ω.node && qt < CH_MAX) queue[qt++] = c->ω.node;   /* IDX-UNIFY: failure edges (je ω) must reach the walk — else/fail blocks were unemitted (rung16_subscript_sub_fail) */
        if (ir_is_generator_kind(c->op) && c->ω.node && qt < CH_MAX) queue[qt++] = c->ω.node;
        if (c->op == IR_SUSPEND && c->n_operands > 1 && c->operands[1] && qt < CH_MAX) queue[qt++] = c->operands[1];
        if (c->op == IR_CREATE && c->n_operands > 0 && c->operands[0] && qt < CH_MAX) queue[qt++] = c->operands[0];
        if (c->op == IR_MOVE_LABEL && c->n_operands > 0 && c->operands[0] && qt < CH_MAX) queue[qt++] = c->operands[0];
        if (c->op == IR_MOVE_LABEL && c->n_operands > 1 && c->operands[1] && qt < CH_MAX) queue[qt++] = c->operands[1];
        /* IR_REPALT operands ([0]=e root, [1]=e chain entry) — same latent hole the ml enqueue closed for
           alternation (2026-07-01 wholesale audit): without this the repeated expression is never BFS-
           discovered, the intercept's e_entry scan misses, e_lbl degrades to node_ω, REPALT fails instantly. */
        if (c->op == IR_REPALT && c->n_operands > 0 && c->operands[0] && qt < CH_MAX) queue[qt++] = c->operands[0];
        if (c->op == IR_REPALT && c->n_operands > 1 && c->operands[1] && qt < CH_MAX) queue[qt++] = c->operands[1];
    }
    bb_label_t **lbls  = (bb_label_t **)alloca(sizeof(bb_label_t *) * n);
    bb_label_t **betas = (bb_label_t **)alloca(sizeof(bb_label_t *) * n);
    bb_label_t **ra_y  = (bb_label_t **)alloca(sizeof(bb_label_t *) * n);
    bb_label_t **ra_t  = (bb_label_t **)alloca(sizeof(bb_label_t *) * n);
    for (int i = 0; i < n && g_flat_chain_set_n < FLAT_CHAIN_SET_MAX; i++) g_flat_chain_set[g_flat_chain_set_n++] = nodes[i];
    /* TE-4: the resume-pointer slot for generator procs with IR_SUSPEND is a LOWER GRANT (graph->resume_slot,
       placed by ir_drive_slot_assign at the 16-aligned tmp high-water mark — the identical position the old
       emit-time carve computed from the cursor). Per-chain gating preserved: set only for chains that contain
       an IR_SUSPEND, only when g_gen_proc_active. */
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
    /* Emit resume-slot initialization for generator procs: store first-suspend's β into resume slot.
       This code lands in α_body (after the prologue) so it runs on the very first call. */
    if (g_suspend_resume_slot >= 0 && g_gen_proc_active) {
        for (int _si = 0; _si < n; _si++) if (nodes[_si]->op == IR_SUSPEND) {
            if (g_is_text) {
                char _init[256];
                snprintf(_init, sizeof _init,
                    "lea rax, [rip + %s]\nmov qword ptr [r12 + %d], rax\n",
                    betas[_si]->name, g_suspend_resume_slot);
                emit_text_n(_init, strlen(_init));
            } else {
                /* BINARY: lea rax, [rip + beta]  →  48 8D 05 <rel32>
                           mov [r12 + slot], rax  →  49 89 84 24 <slot_u32> */
                ef_b3(0x48, 0x8D, 0x05); bb_emit_patch_rel32(betas[_si]);
                ef_b4(0x49, 0x89, 0x84, 0x24); bb_emit_u32((uint32_t)(unsigned)g_suspend_resume_slot);
            }
            break;
        }
    }
    /* LIMIT counter init: bb_limit's counter at [op_off+16] is check-BEFORE-yield from 0; the frame is
       NOT zeroed, so zero it once per chain α-entry, before the spine runs. This is the "chain pre-pass"
       bb_limit_init's own header comment promised — it had ZERO call sites until now (found by the
       2026-07-01 wholesale audit; empty-output symptom was garbage counter >= t at first gate entry). */
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
        int gamma_is_beta = (nodes[i]->γ.sz[0] == (char)0xce && (unsigned char)nodes[i]->γ.sz[1] == 0xb2); /* "β" UTF-8 = CE B2 */
        int omega_is_beta = (nodes[i]->ω.sz[0] == (char)0xce && (unsigned char)nodes[i]->ω.sz[1] == 0xb2);
        for (int k = 0; k < n; k++) if (nodes[k] == gtgt) {
            /* edge carries its own port: the graph says α or β, the BFS never reconstructs it positionally */
            node_γ = gamma_is_beta ? betas[k] : lbls[k];
            break;
        }
        if (nodes[i]->γ.node == NULL || nodes[i]->γ.node->op == IR_SUCCEED) node_γ = &lbl_γ;
        if (nodes[i]->γ.node && nodes[i]->γ.node->op == IR_FAIL) node_γ = &lbl_ω;
        int omega_resolved = 0;
        for (int k = 0; k < n; k++) if (nodes[k] == otgt) { node_ω = omega_is_beta ? betas[k] : lbls[k]; omega_resolved = 1; break; }
        if (!omega_resolved) node_ω = &lbl_ω;
        /* flat_drive_repalt edge ownership: if THIS node is some REPALT's e_root (operand[0]), its success is the yield stub (copy value, yielded:=1, jmp repalt-γ) and its failure is the exhausted-test
           stub (yielded ? restart : repalt-ω) — JCON ir_a_RepAlt's e.success/e.failure chunks (irgen.icn:215-219).  Lower left e_root's γ NULL (would mis-default to lbl_γ) and ω at the enclosing target
           (would skip the test); the redirect happens HERE, at edge-resolution time, never by writing the graph. */
        for (int r = 0; r < n; r++) if (nodes[r]->op == IR_REPALT && nodes[r]->n_operands > 0 && nodes[r]->operands[0] == nodes[i]) { node_γ = ra_y[r]; node_ω = ra_t[r]; break; }
        /* Pre-propagate IR_CONJUNCTION slot: if the join has operand[0] with a known tmp slot,
           register it NOW so downstream nodes (e.g. write's arg-slot lookup) see it. */
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
        /* IR_CREATE (RUNG 3, Claude Sonnet 2026-07-01): operand[0] = the coexpression body's entry node
           (RUNG 1's lowering: "create.operand[0] = body's entry node (coswitch target)"). No existing
           mechanism maps an arbitrary IR_t* to its own α-label from a DIFFERENT node's driver case — the
           label arrays (lbls[]/betas[]) are local to this function, indexed by nodes[]-position, only
           ever resolved via linear scan at the point a node's OWN edges are stitched (see the gtgt/otgt
           scans above and IR_REPALT's e_entry scan below, which this follows exactly). Resolve here,
           store the label STRING (not the bb_label_t*, since bb_create.cpp's TU doesn't have bb_label_t
           in scope) into g_emit.op_sval_lbl (declared, previously unused anywhere) for the template to
           LEA against. If operand[0] isn't found in this chain's nodes[] (shouldn't happen per RUNG 1's
           own lowering, which always sets it — but this scan degrades safely to NULL rather than a wild
           pointer if it ever does), the template's own guard (op_sval_lbl == NULL) bombs loudly rather
           than emitting a bad LEA target. */
        if (nodes[i]->op == IR_CREATE && nodes[i]->n_operands > 0 && nodes[i]->operands[0]) {
            IR_t *body_entry = nodes[i]->operands[0];
            g_emit.op_sval_lbl = NULL;
            g_create_body_entry = NULL;
            for (int k = 0; k < n; k++) if (nodes[k] == body_entry) { g_emit.op_sval_lbl = lbls[k]->name; g_create_body_entry = lbls[k]; break; }
        }
        /* IR_MOVE_LABEL (unbounded alternation): operand[0] = the arm's resume-point node, captured by lower_alt
           as post-arm cx->beta (or the arm's ωj when the arm is non-resumable). ival=1 means the resume enters
           the target's β (a real generator inside the arm produces its next value); ival=0 means the target's α
           (JCON's "a non-generator's resume IS its failure" — spent arm cascades to the next arm's fresh entry,
           or to the alternation's ω). Sentinel targets map to the chain exits exactly as the gtgt/otgt edge
           resolution above does (IR_FAIL→lbl_ω, IR_SUCCEED→lbl_γ). Same lbls[]/betas[]-are-local-to-this-function
           constraint the IR_CREATE block documents; same t0-port threading. */
        if (nodes[i]->op == IR_MOVE_LABEL && nodes[i]->n_operands > 0 && nodes[i]->operands[0]) {
            IR_t *rtgt = nodes[i]->operands[0];
            int wantb = (int) IR_LIT(nodes[i]).ival;
            if (rtgt->op == IR_FAIL) g_move_label_tgt = &lbl_ω;
            else if (rtgt->op == IR_SUCCEED) g_move_label_tgt = &lbl_γ;
            else for (int k = 0; k < n; k++) if (nodes[k] == rtgt) { g_move_label_tgt = wantb ? betas[k] : lbls[k]; break; }
        }
        /* IR_MOVE_LABEL success rides THROUGH the sibling IR_INDIRECT_GOTO's γ — that edge is JCON's
           p.ir.success, the ONE label a caller re-points when it wires the alternation's value node onward
           (the wire-later pattern: lower left with γ=NULL, γ_to(*res, next) afterward — *res is ig, so ig.γ
           holds the final continuation; each ml's own γ was frozen at lower time and may be NULL/stale).
           Emit-time READ of the finished graph, never a write; same stamp + sentinel logic as gtgt above. */
        if (nodes[i]->op == IR_MOVE_LABEL && nodes[i]->n_operands > 1 && nodes[i]->operands[1]) {
            IR_t *igp = nodes[i]->operands[1];
            IR_t *sg = igp->γ.node;
            int sg_is_beta = (igp->γ.sz[0] == (char)0xce && (unsigned char)igp->γ.sz[1] == 0xb2);
            if (sg == NULL || sg->op == IR_SUCCEED) node_γ = &lbl_γ;
            else if (sg->op == IR_FAIL) node_γ = &lbl_ω;
            else for (int k = 0; k < n; k++) if (nodes[k] == sg) { node_γ = sg_is_beta ? betas[k] : lbls[k]; break; }
        }
        /* IR_REPALT: intercept before emit_drive — flat_drive_repalt (defined above drive_unowned) emits the four
           stubs (α clear+jmp-e / yield / exhausted-test / β pump); e_root's γ/ω were redirected onto ra_y/ra_t by
           the edge-resolution scan above.  JCON ir_a_RepAlt, irgen.icn:206-229. */
        if (nodes[i]->op == IR_REPALT) {
            flat_drive_repalt(nodes, n, i, lbls, betas, ra_y, ra_t, node_γ, node_ω);
            continue;  /* skip normal emit_drive */
        }
        /* IR_LIMIT: set g_limit_gen_beta so emit_drive's IR_LIMIT arm sees the generator's β. */
        if (nodes[i]->op == IR_LIMIT) {
            IR_t *gen = nodes[i]->n_operands > 0 ? nodes[i]->operands[0] : NULL;
            if (gen) for (int k = 0; k < n; k++) if (nodes[k] == gen) { g_limit_gen_beta = betas[k]; break; }
        }
        emit_drive(nodes[i], node_γ, node_ω, betas[i]);
    }
    emit_label_define_bb(&lbl_β);
    if (g_suspend_resume_slot >= 0 && g_gen_proc_active) {
        /* Indirect goto: jmp qword ptr [r12 + resume_slot] */
        if (g_is_text) {
            char _ind_jmp[64];
            snprintf(_ind_jmp, sizeof _ind_jmp, "jmp qword ptr [r12 + %d]\n", g_suspend_resume_slot);
            emit_text_n(_ind_jmp, strlen(_ind_jmp));
        } else {
            /* BINARY: jmp qword ptr [r12 + slot]  →  49 FF A4 24 <slot_u32> */
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
/*--------------------------------------------------------------------------------------------------------------------*/
/* Drive nd via walk_bb_flat, but if nd is the first arm of a γ/ω-threaded inline alt
   chain (PAT_ALT with no operand_aux), drive all arms with cascading ω-labels. */
/*--------------------------------------------------------------------------------------------------------------------*/
static int g_in_prebuild = 0;
/*--------------------------------------------------------------------------------------------------------------------*/
/*============================================== CHAIN OPERAND PRE-WALK ==============================================*/
static int descr_chain_arity(const IR_t *n) {
    switch (n->op) {
    case IR_LIT_INTEGER: case IR_LIT_STRING: case IR_LIT_REAL:
    case IR_VAR:   case IR_KEYWORD: return 0;
    case IR_BINOP: case IR_TO: return 2;
    case IR_TO_BY: return 3;
    case IR_MAKE_LIST: return n->n_operands;
    case IR_CONJUNCTION: case IR_GOTO: return 0;
    case IR_UNOP: case IR_UNOP_TEST: return 1;
    case IR_ASSIGN: return 1;
    case IR_RETURN: return 1;
    case IR_CALL_PROC_STAGED: case IR_CALL_USERPROC: case IR_CALL_BYNAME: case IR_CALL_BUILTIN: case IR_CALL_GVAR_USERPROC:
    case IR_CALL:  return n->n_operands;
    case IR_PROC_GEN: return 0;
    case IR_SCAN_TAB: case IR_SCAN_MOVE: case IR_SCAN_POS:
    case IR_SCAN_UPTO: case IR_SCAN_ANY: case IR_SCAN_MANY:
    case IR_SCAN_FIND: case IR_SCAN_MATCH: case IR_SCAN_BAL: return 0;
    default:       return -1;
    }
}
static void descr_chain_operand_refs(IR_t *entry) {
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
        if ((c->op == IR_SUBSCRIPT || c->op == IR_RANDOM || c->op == IR_DEREF || c->op == IR_ASSIGN_VAR || c->op == IR_REV_ASSIGN_VAR || c->op == IR_SWAP_VAR || c->op == IR_CALL_VALUE) && c->ω.node && sv < 512) stkv[sv++] = c->ω.node;
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
        if ((c->op == IR_SUBSCRIPT || c->op == IR_RANDOM || c->op == IR_DEREF || c->op == IR_ASSIGN_VAR || c->op == IR_REV_ASSIGN_VAR || c->op == IR_SWAP_VAR || c->op == IR_CALL_VALUE) && c->ω.node && sv < 512) stkv[sv++] = c->ω.node;
        if (ir_is_generator_kind(c->op) && c->ω.node && sv < 512) stkv[sv++] = c->ω.node;
        if (c->γ.node && sv < 512) stkv[sv++] = c->γ.node;
    }
    IR_t *stk[512]; int sp = 0;
    for (int i = 0; i < nc; i++) {
        IR_t *n = chain[i];
        int ar = descr_chain_arity(n);
        if (ar < 0) { sp = 0; continue; }
        else if (ar >= 1 && sp >= ar) { if (n->n_operands < ar) { n->n_operands = 0; for (int k = ar; k >= 1; k--) ir_operand_push(n, stk[sp - k]); } sp -= ar; }
        else if (ar >= 1) { sp = 0; }
        stk[sp++] = n;
    }
}
/*======================================= CHAIN BUILDERS & WHOLE-GRAPH PASSES ========================================*/
bb_box_fn descr_flat_chain_build(IR_t *entry) {
    if (!entry) return NULL;
    descr_chain_operand_refs(entry);
    bb_buf_t buf = bb_alloc(FLAT_BUF_MAX);
    if (!buf) return NULL;
    g_flat_node_id = 0; g_bb_slotmap_n = 0;
    g_flat_chain_set_n = 0;
    emitter_init_binary(buf, FLAT_BUF_MAX);
    codegen_flat_chain_body(entry, "pat_flat");
    int nbytes = emitter_end();
    extern int bb_emit_overflow;
    if (bb_emit_overflow || nbytes <= 0 || nbytes > FLAT_BUF_MAX) { bb_free(buf, FLAT_BUF_MAX); return NULL; }
    bb_seal(buf, (size_t)nbytes);
    bb_pool_trim_last(buf, FLAT_BUF_MAX, (size_t)nbytes);
    return (bb_box_fn)buf;
}int descr_flat_chain_build_text(IR_t *entry, FILE *out, const char *prefix) {
    if (!entry) return 1;
    descr_chain_operand_refs(entry);
    g_bb_slotmap_n = 0;
    g_flat_chain_set_n = 0;
    emitter_init_text(out, TEXT_MODE_INVOCATION);
    int rc = codegen_flat_chain_body(entry, prefix);
    emitter_end();
    return rc;
}
bb_box_fn descr_flat_chain_build_proc(IR_t *entry, const char **pnames, int np) {
    if (!entry) return NULL;
    descr_chain_operand_refs(entry);
    bb_buf_t buf = bb_alloc(FLAT_BUF_MAX);
    if (!buf) return NULL;
    g_flat_node_id = 0; g_bb_slotmap_n = 0;
    (void)pnames; (void)np;
    /* TE-4: params + locals + resume are LOWER grants in the graph's vslots table (ir_drive_slot_assign) —
       the entry-point intern loop and the cursor seed die with the allocator. LOWER's total IS the frame:
       published for rt_proc_set_frame_bytes (raises the runtime arena default when a proc outgrows it). */
    g_last_flat_frame_bytes = g_emit_cfg ? g_emit_cfg->jcon_value_region : 0;
    emitter_init_binary(buf, FLAT_BUF_MAX);
    codegen_flat_chain_body(entry, "proc_flat");
    int nbytes = emitter_end();
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
    g_bb_slotmap_n = 0;
    (void)pnames; (void)np;
    /* TE-4: grants in the graph's vslots table — same as descr_flat_chain_build_proc; see the comment there. */
    g_last_flat_frame_bytes = g_emit_cfg ? g_emit_cfg->jcon_value_region : 0;
    char prefix[256];
    snprintf(prefix, sizeof(prefix), "proc_%s", pname);
    emitter_init_text(out, TEXT_MODE_INVOCATION);
    fprintf(out, "  .globl %s_\316\261\n", prefix);
    int rc = codegen_flat_chain_body(entry, prefix);
    emitter_end();
    return rc;
}
/*====================================================================================================================*/
bb_box_fn gvar_flat_chain_build(IR_graph_t *g) {
    fprintf(stderr, "GROUND ZERO: %s not implemented (Icon-only reset)\n", "gvar_flat_chain_build"); abort(); }
/*--------------------------------------------------------------------------------------------------------------------*/
int gvar_flat_chain_build_text(IR_graph_t *g, FILE *out, const char *prefix) {
    fprintf(stderr, "GROUND ZERO: %s not implemented (Icon-only reset)\n", "gvar_flat_chain_build_text"); abort(); }
/*--------------------------------------------------------------------------------------------------------------------*/
bb_box_fn gvar_flat_chain_build_at(IR_graph_t *g, IR_t *entry_node, const char *prefix) { fprintf(stderr, "GROUND ZERO: %s not implemented (Icon-only reset)\n", "gvar_flat_chain_build_at"); abort(); }
/*--------------------------------------------------------------------------------------------------------------------*/
void gva_collect_graph(IR_graph_t *g) { fprintf(stderr, "GROUND ZERO: %s not implemented (Icon-only reset)\n", "gva_collect_graph"); abort(); }
/*--------------------------------------------------------------------------------------------------------------------*/
void gva_collect_icon_globals(void) {
    extern const char *global_names[]; extern int global_count;
    for (int i = 0; i < global_count; i++) if (global_names[i]) (void)gva_collect_var(global_names[i]);
}
/*--------------------------------------------------------------------------------------------------------------------*/
int gvar_flat_chain_build_text_at(IR_graph_t *g, IR_t *entry_node, FILE *out, const char *prefix) {
    fprintf(stderr, "GROUND ZERO: %s not implemented (Icon-only reset)\n", "gvar_flat_chain_build_text_at"); abort(); }
/*--------------------------------------------------------------------------------------------------------------------*/
bb_box_fn bb_build_flat(IR_t *nd) { fprintf(stderr, "GROUND ZERO: %s not implemented (Icon-only reset)\n", "bb_build_flat"); abort(); }
/*--------------------------------------------------------------------------------------------------------------------*/
int codegen_flat_build(IR_t *nd, FILE *out, const char *prefix) { fprintf(stderr, "GROUND ZERO: %s not implemented (Icon-only reset)\n", "codegen_flat_build"); abort(); }
/*--------------------------------------------------------------------------------------------------------------------*/
#define PL_CATCH_MAX 64
static IR_t *g_pl_catch_nodes[PL_CATCH_MAX];
static int   g_pl_catch_n = 0;
#ifdef __cplusplus
}
#endif

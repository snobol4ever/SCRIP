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
void          (*g_emit_chain_posthook)(void) = NULL;   /* RTX-FUNC-0: one-shot hook called inside emit_chain's binary session after codegen_flat_chain_body; cleared on fire so proc chains never trigger it */
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
extern "C" const char * bb_kind_name(int op) { return flat_label_kind((IR_e)op); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* FLOATERS (Lon 2026-08-13): RETURN/FRETURN/NRETURN are program-wide SINGLETON blocks — output ONCE, jumpable from anywhere, labeled by their own bare names (a node-numbered label is a chain resident,
 * not a floater).  emit_floater_kind is THE ONE membership authority: SAVE_RESTORE ival=1/2 (sno_build_graph's RETURN/FRETURN mint) and the NRETURN head (the lit_string whose γ is the SNO$NRET call).
 * The collector EXCLUDES floaters from every chain's γ/ω expansion (RPO_PUSH conjunct) and the graph-root call seeds them as its FINAL root blocks, so they land once, near main, with main at the
 * bottom of the file.  Cross-chain jumps resolve through the ONE shared bb_label_t per name (emit_floater_label), both media.  emit_floater_member additionally covers the NRETURN call node so the
 * referenced-scan at the seed does not count the floaters' own internal wiring as an outside reference.  Bombs today; the coming-out restore (the pop-rbp dance) is a LATER rung — no RBP here yet. */
static int emit_floater_kind(const IR_t * n) { if (!n) return 0; if (n->op == IR_SAVE_RESTORE && IR_LIT(n).ival == 1) return 1; if (n->op == IR_SAVE_RESTORE && IR_LIT(n).ival == 2) return 2; if (n->op == IR_LIT_STRING && n->γ.node && n->γ.node->op == IR_CALL && IR_LIT(n->γ.node).sval && !strcmp(IR_LIT(n->γ.node).sval, "SNO$NRET")) return 3; return 0; }
static int emit_floater_member(const IR_t * n) { if (!n) return 0; if (emit_floater_kind(n)) return 1; if (n->op == IR_CALL && IR_LIT(n).sval && !strcmp(IR_LIT(n).sval, "SNO$NRET")) return 1; return 0; }
static bb_label_t * g_flt_lbl[4] = {0, 0, 0, 0};
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
/* SN4-M34-5a (2026-08-12): DEFINE-entry double-emission fix.  A plain DEFINE mints TWO citizens that can
 * both reach the SAME shared body anchor: the LBL__<label> pseudo-proc (proc_entry_node = the label's anchor
 * inside main's own graph -- SN4-FLAT-PROC's O(n) shared-graph design, lower_snobol4.c ~2538) gets its OWN
 * standalone emit_chain call and legitimately emits that body once; separately, codegen_flat_chain_body's
 * "pass 1: entry + enterable-chain roots" step (the ORPHAN-PROOF group-root pull-in, HQ s26) walks EVERY
 * anchor the ZLS group registry knows about for whichever graph g_emit_cfg currently names -- and for a
 * chain whose own `entry` param happens to equal its own `g->entry` (true of EVERY single-node graph,
 * including the tiny call-stub the SAME DEFINE also mints -- sno_build_call_stub, IR_SAVE_RESTORE ->
 * IR_GOTO_DEFERRED), that guard is vacuously satisfied and, when g_emit_cfg for that emit_chain call is
 * MAIN's own graph (main's own top-level chain), the pull-in walks the SAME anchor LBL__<label> already
 * emitted standalone -- re-emitting the whole body a second time under main's chain.  Every by-name-call
 * template inside the re-walked body mints its `.Lbynamefnzd<nid>` label from _.nid a SECOND time (bb_node_id
 * is dense/collision-free -- the SAME node gets the SAME nid both times, correctly -- so the label is
 * IDENTICAL, and `as` rejects the duplicate definition).  Root cause is NOT a nid-uniqueness bug: it is two
 * independent top-level emit_chain calls both structurally reaching the identical IR_t* subgraph.  FIX: the
 * group-root pass must skip any anchor that some EARLIER emit_chain call in this same compilation already
 * used as its OWN entry (i.e. already has a dedicated standalone emission, exactly the LBL__ case) -- that
 * anchor's body is already transfer-reachable and already emitted; pulling it in again is pure duplication,
 * never new coverage.  Anchors with no dedicated proc (the ordinary case this pass exists for) are unaffected
 * -- they are never marked, so they still pull in exactly as before.  ONE AUTHORITY: emit_chain marks ITS OWN
 * raw (pre-entry-chase) `entry` parameter at call start -- the same pointer identity zls_group_mark_anchor
 * registered, since both derive from the same label anchor (bb_label_landing/anchor[i]) -- so the mark and
 * the group registry's returned anchors compare by identical IR_t* with no chase-drift possible. */
static IR_t * g_chain_entry_key[65536];
static int    g_chain_entry_n = 0;
void emit_chain_mark_entry_emitted(IR_t * entry) {
    if (!entry) return;
    for (int i = 0; i < g_chain_entry_n; i++) if (g_chain_entry_key[i] == entry) return;   /* already marked (re-entrant/defensive; emit_chain runs once per proc in practice) */
    if (g_chain_entry_n < 65536) g_chain_entry_key[g_chain_entry_n++] = entry;
}
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
void emit_fb_divergence_check(void) { if (fbdiv_mode < 0) { const char * e = getenv("SCRIP_FB_DIVERGE"); fbdiv_mode = (e && *e == '1') ? 1 : 0; } if (!fbdiv_mode) return; if (!fbdiv_hooked) { fbdiv_hooked = 1; atexit(fbdiv_report); } if (emit_jmp_pin_legacy() == emit_rec_pin()) return; fbdiv_n++; fprintf(stderr, "FB-DIVERGE #%d graph=%s data_fb=%s rec_fb=%s deep=%d pat=%d gen=%d genproc=%d rescall=%d\n", fbdiv_n, g_emit.flat_lbl_α ? g_emit.flat_lbl_α : "<anon>", emit_jmp_pin_legacy() ? "rsp" : "rsp", emit_rec_pin() ? "rsp" : "rsp", g_emit.flat_deep_arrival, g_emit.flat_pat, g_emit.flat_gen, g_gen_proc_active, g_resumable_callable_active); }
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
static int earn_hazard_in(const IR_t * nd, int depth) {                                          /* EARN-1 (___-EARN s47): hazardous material test -- OPAQUE (*E / runtime pattern value) or ARBNO (unknown count => unknown size, ONE class per s28).  Conservative: null/over-deep = hazardous. */
    if (!nd || depth > 32) return 1;
    if (nd->op == IR_MATCH_ARBNO) return 1;
    if (nd->op == IR_MATCH_DEFER && !nd->pat_static) return 1;                                   /* pat_static calloc-default 0 covers the s29 BINOP dropped-operand trap conservatively */
    if (nd->op == IR_MATCH_VALUE) return 1;
    for (int i = 0; i < nd->n_operands; i++) if (earn_hazard_in(nd->operands[i], depth + 1)) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int frame_need_of(const IR_t * nd) {                                                      /* ⭐ EARN-1 SLICE 1 -- THE ONE AUTHORITY, DORMANT (no reader).  THE LAW: a cell needs a frame iff its byte distance to RSP is not a compile-time constant at some reading site.  Verdict map + placeholder caveats documented on op_frame_need (emit.h). */
    if (!nd) return 0;
    switch (nd->op) {
    case IR_MATCH_ARBNO:        return 1;
    case IR_MATCH_FENCE1:       return 1;
    case IR_MATCH_ASSIGN_IMM:
    case IR_MATCH_ASSIGN_COND:
    case IR_MATCH_ASSIGN_SAVE:  return earn_hazard_in(nd, 0);
    default:                    return 0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void bb_prepare(IR_t *nd) {
    if (!PLATFORM_X86) return;
    g_emit.bb_ls = NULL;
    g_emit.bb_rs = NULL;
    g_emit.bb_op_lbl = NULL;
    g_emit.bb_lk = -1;
    g_emit.op_frame_need = frame_need_of(nd);                                                    /* EARN-1: staged at the ONE choke; DORMANT -- nothing reads it yet */
    { static int _ed = -1; if (_ed < 0) { const char * e = getenv("SCRIP_EARN_DIAG"); _ed = (e && *e == '1') ? 1 : 0; }
      if (_ed && nd) fprintf(stderr, "[EARN] op=%d need=%d haz=%d pat_static=%d n_op=%d\n", nd->op, g_emit.op_frame_need, earn_hazard_in(nd, 0), nd->pat_static, nd->n_operands); }
    { static int _cd = -1; if (_cd < 0) { const char * e = getenv("SCRIP_CLASS_DIAG"); _cd = (e && *e == '1') ? 1 : 0; }
      if (_cd && nd) { extern const char * bb_op_name(IR_e);
        if (emit_rec_pin() || g_emit.op_stmt_dyn) fprintf(stderr, "[CLS:NOFIX] op=%s(%d) pin=%d dyn=%d\n", bb_op_name(nd->op), (int)nd->op, emit_rec_pin(), g_emit.op_stmt_dyn);
        if (g_emit.op_frame_need && !(nd->op == IR_MATCH_FENCE1 && nd->n_operands == 0)) fprintf(stderr, "[CLS:NOWHACK] op=%s(%d)\n", bb_op_name(nd->op), (int)nd->op); } }   /* s65b BUG-HUNT LEDGERS (Lon in-chat: "SCRIP knows at compile time... NO FIXED offset... NO WHACK power for unbounded growth... report that as one class and keep track of a list"): the two COMPILE-TIME-KNOWABLE classes, reported at the ONE staging choke.  NOFIX = the s52/s53 dyn-offset predicate (emit_rec_pin || op_stmt_dyn) that x86_rsp_slide_known's deleted 0-arm read -- the knowledge survives, the [rsp + -1] sentinel does not.  NOWHACK = frame_need_of's need=1 minus FENCE1 (its keeper mechanism IS whack power -- the EARN-2 exclusion precedent).  Env-gated function-local static cache, stderr only, byte-inert when off -- the sanctioned SCRIP_EARN_DIAG / [ZD-GAP] shape.  s67 SPLITS THE FENCE1 EXCLUSION: it is sound ONLY for BARE fence (n_operands==0), whose keeper mechanism really IS whack power -- the ABORT drain, and G22/G23 pass on the identical failure edge.  FENCE(P) (n_operands>0) is the OTHER construct (manual Ch.19: backup ALWAYS PASSES THROUGH FENCE() and does NOT abort), so its whack power is never exercised on that edge; the blanket exclusion was masking a genuine frame_need_of==1 -- measured on H26/H27, which reported nw=0 while the classifier said need=1 at op=96. */
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
    { IR_t *_gt = nd->γ.node; int _gg = 0; while (_gt && _gt->op == IR_GOTO && _gg++ < 128) _gt = _gt->γ.node; IR_t *_ot = nd->ω.node; int _og = 0; while (_ot && _ot->op == IR_GOTO && _og++ < 128) _ot = _ot->γ.node; if (g_emit_cfg && g_emit_cfg->icn_cells_graph && _gt && _ot && _gt == _ot && (unsigned char)nd->ω.sz[0] == 0xce && (unsigned char)nd->ω.sz[1] == 0xb2) return 0; }   /* ZK-3 EVERY-BODY-WRITE: bypass write-slot optimization in the every-body topology (gamma=IR_SUCCEED, omega=beta-tag). bb_call_write_slot does not have the op_sb=1 arm that routes both exits to beta_trampoline; falling back to the generic CALL_ROUTE_FN path (bb_call_fn_str) picks up that arm correctly. The write-slot routes are pure optimizations; returning 0 is always correct. GATED icn_cells_graph+IR_SUCCEED+beta-omega: SN4/Prolog and top-level write() calls (omega not beta) are byte-identical. ONE LINE ONLY per s22k law. */
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
static int g_zd_stage, g_zd_arm, g_zd_k, g_zd_gpop, g_zd_wpop, g_zd_wsteal, g_zd_read[6], g_zd_kind[6], g_zd_ztail, g_zd_zunder, g_zd_zpat, g_zd_zfc;   /* g_zd_zfc (L-3b, this session): the choke-staged twin of op_zfc -- see emit.h's op_zfc field comment for the full mechanism. */
static int g_arbk16_stmt = 0;   /* ⭐ ARB-LON-K16: per-statement latch -- set by the ARBNO prelude when it routes FRAMELESS_K, consumed+cleared at the ZD-1 choke on STATEMENT_END/STATEMENT (defensive clear at STATEMENT_BEGIN), staging op_stmt_dyn so the γ release is the mechanism-2 lea whack instead of the static add */
static long g_zd_suspend_uclaim = 0;   /* ICN-FR-5: staging twin for op_suspend_stmt_uclaim; field survives M-3 (struct-end ABI append); UCLAIM staging source (zd_uk) deleted by M-3 -- stays 0 pending ICN-FR re-plumb */
/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int zd_sr_role(const IR_t * nd) { int64_t v = IR_LIT(nd).ival; return (v == 1 || v == 2 || v == 3 || v == 4) ? (int)v : 0; }   /* ⭐ ONE AUTHORITY for the IR_SAVE_RESTORE role, per the s22k ZD-K law ("grep the RULE'S SHAPE, not the variable's name" -- that law was written after a THIRD spelling of K survived a two-site collapse, so this rule gets one site the first time it is needed twice).  IR.h's sval/ival/dval are ONE union: roles 1-4 (RETURN floater / FRETURN floater / WIRE-ADOPT / TINY-REAL s58 per-DEFINE alpha-beta SHIM) write ival, role 0 (the CALL2BB call-site OPEN box) writes sval, an lp_strdup heap pointer that is never 1..4 -- so the value read back IS the role.  Consumers: the dispatch's op_ival normalization below, and zd_wl_kind's ZD-SR admission (role 4 = transparent like 1-3: carves and releases its OWN record, no spine cells). */
/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int zd_k(IR_t * nd);   /* forward: THE ONE K AUTHORITY (defined below, s22k law) -- consulted by the ARBNO-FRAMELESS body-K0 staging scan so the K=0 list is never spelled twice */
static int cap_ok_at(IR_t * base, int bi) { for (int _j = 0; _j < g_emit_cfg->n; _j++) { IR_t * _o = g_emit_cfg->all[_j]; if (_o && _o->op == IR_MATCH_ARBNO && _o->n_operands >= 3) { int _a = -1, _b = -1; for (int _k = 0; _k < g_emit_cfg->n; _k++) { if (g_emit_cfg->all[_k] == _o->operands[1]) _a = _k; if (g_emit_cfg->all[_k] == _o->operands[2]) _b = _k; } if (_a > _b) { int _t = _a; _a = _b; _b = _t; } if (_a >= 0 && bi >= _a && bi <= _b) return 0; } } (void)base; return 1; }   /* ⭐ OS-2·SLICE-1 per-node predicate half of THE ONE ROUTING AUTHORITY: ARBNO body span [operands[1],operands[2]] declines (multi-instance suspension re-enters the SAME static node; per-node slots are per-activation only outside ARBNO -- SLICE-2/3 customers keep ZB-FC-3c legacy). */
static int cap_anchor_of(IR_t * nd) { (void)nd; return 0; }   /* ⛔ s53 RSP-ONLY: ANCHOR ARM RETIRED (was OS-2·SLICE-1).  Its premise -- a depth-immune REAL frame behind FR(op_off) region slots -- died with the pin: under sliding rsp the SAVE and COND spellings of the ONE region address diverge by every carve between them (r3/r5 A/B conviction: proc graph wrote [rsp+192] read [rsp+176] while the ZD arm at main level wrote/read [rsp+0]/[rsp+0] exactly).  The ZD capture path (SCRIP_ZD_CAP: SAVE K=16 own cell + COND ZOPQ depth-diff) is the FORTH-correct ONE system and now serves every class.  Original body with full provenance at git a50037ae^ and rung notes above. */   /* ⭐ OS-2·SLICE-1 (GOAL-SN4-ZETA-MECH ONE-SYSTEM): THE ONE ROUTING AUTHORITY, now returning the 1-BASED REGION SLOT k (0 = declined/killswitched) -- env read HERE only (SCRIP_OS_CAP=0 = byte-identical legacy, killswitch law); the template keys purely on the staged op_cap_anchor so the ends cannot half-flip (ARBNO-K16 precedent verbatim).  k = 1 + count of prior anchored SAVEs in cfg order; IMM/COND resolve through operands[1] (the SAVE twin) so the pair shares one slot BY CONSTRUCTION -- producer symmetric, same bracket, same k.  Slot HOME by graph class (template caphome()): legacy deep MAIN = [___ - 64 - 16k] inside the GLUE-O enter carve (64 clears the stfh HKQ zone at ___-48..-8); carve-then-pin classes (blob/lcl_proc/zframe/gen) = FR(op_off) raw against their REAL frames.  GRAPH DECLINES: any IR_MATCH_FENCE1/IR_MATCH_ABORT present -- both whack rsp to the ___ floor mid-graph (law-4 users), which would free the region while slots are live; fence+capture composition is a later slice.  The m4-SEGV that forced this shape: raw graph-flat FR offs (272/320/400) against the single-bracket main ___ overshot the 8-byte token carve into CRT territory -- slot 400 landed INSIDE environ[8], a dword cursor store smashed the pointer array, and getenv walked it to a SEGV (measured in gdb; the s22r/s23a envp-corruption class verbatim, extending the PIN-REBASE evidence: the LP-1 flat_all_zd=8 "region unused" premise is FALSIFIED by pinned-FR region readers -- the match-head quartet at [___+160..232] rides the same slack and is a pre-existing latent of the same class). */
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
      { extern long zw_carve_k(const IR_t *); extern int zc_nofc(void); static int _kc = -1; if (_kc < 0) { const char * e = getenv("SCRIP_NOFC_CARVE"); _kc = (e && *e == '1') ? 1 : 0; } long _k = (zc_nofc() && !_kc) ? 0 : zw_carve_k(nd);   /* ⛔⭐⭐⭐ ATTRIBUTION CORRECTED BY MEASUREMENT (NOFC-SYM-2, s22l-B) -- THE 33-PROGRAM WIN IS THIS LINE, NOT THE NON-POPPING SPINE.  Both halves of NOFC-SYM landed in one commit and the credit went to the wrong one.  MEASURED by reverting THIS SUPPRESSION ALONE, everything else at HEAD, ASLR off: crosscheck m3 308/9 -> 277/40, m4 306/10/1 -> 275/41/1 -- the ENTIRE 33-program pattern win evaporates and the watermark returns to the default regime, while 143_pat_regex_quantified_class and 164_pat_arbno_nested BOTH GO GREEN (164 was crashing in BOTH modes, rc=139; its m3 "PASS" was a stdout-only artifact of flushing correct output before dying, so the DIVERGE=1 figure was itself partly instrument noise).  THE TWO EFFECTS ARE ONE EDIT AND THEY TRADE AGAINST EACH OTHER: suppressing the ZW-1 universal carve takes the match family OFF its self-allocated zls extent and back onto the flat frame, which 33 pattern programs need and 2 do not.  ⛔ SO THE LIVE QUESTION IS NOT "SHOULD NOFC BE THE DEFAULT" -- it is WHICH NODE KINDS' CARVE HELPS AND WHICH HURTS, and that is a per-kind bisect nobody has run.  zw_carve_k already excludes the value spine (its _spine test) and every fc_geom-granted node BY CONSTRUCTION, so everything this line reaches is match-family storage; the discriminator lives INSIDE that set.  `SCRIP_NOFC_CARVE=1` restores the carve under NOFC -- that is the bisect handle: pair it with SCRIP_BB_ONLY/SCRIP_BB_SKIP (zw_carve_k's own per-nid gates) to find the split. */   /* CARVE-ERAD step 1: the predicate that used to live here is now zw_carve_k (zeta_storage.c) so the LOWER-side prefix walk spends the SAME K this hook does -- one authority, per the zw_node_k precedent. */
        if (_k > 0) g_emit.op_fc_bytes = _k; } } }
    /* ZTOS-1 SLIDING-OFFSET AUTHORITY (Lon s21x-o "ONE main function that does graph traversal and calculates the zeta offsets"; "no pre-allocation calculation is necessary").  op_zdepth is that calculation,
     * and it is one line because the traversal that needs to happen is the one the emitter is ALREADY doing: walk_bb_node_inner is the single choke every template invocation passes through, and at this point
     * the node's own K is final.  The alpha carve about to be emitted (x86_port_hook's FORTH arm / bb_glue_flat_enter) subtracts exactly op_fc_bytes, so that IS the depth every TOS read inside this body must
     * add back.  Set unconditionally from the same field the allocator spends, so the allocator and the accessor CANNOT hold two opinions -- the ONE-K-AUTHORITY discipline (zw_node_k) extended to the address
     * side.  Zero when the box carves nothing, which is what makes every unconverted template byte-identical and lets the family-by-family conversion crawl safely. */
    g_emit.op_zdepth = x86_fc_on() ? (int)g_emit.op_fc_bytes : 0;   /* ⭐ ZW-RB-1 Part 4b (CORRECTED W-1): blob closure members (op_zw2=1) read FR slots at [rsp+off+8]=[α_base-Kc+off] -- inside the carved region. Old value was 8+Kc which placed FR at [α_base+off] = above α_base = caller territory (the W-1 SEGV root cause). op_mech2_kc set at MATCH_BEGIN, persists (not cleared per-node). Checked BEFORE the fc_on path so mech-2 wins. */   /* ZW-1 UNIVERSAL CARVE (Lon s21x-m "across the board, then crawl"): every previously-ungranted node self-allocates its whole zls extent at alpha and releases at every omega, THROUGH THE EXISTING x86_alpha/x86_omega HOOK (x86_asm.h ~1839 arms on op_fc_bytes alone) -- op_fc_base stays -1 so this is CARVE-ONLY: zero FR/FRQ spellings change, the one new machine effect is the rsp motion.  fc_geom-granted kinds keep their per-kind preamble geometry untouched (the !fc_geom conjunct); the FOUR-construct family (HEAD/FENCE1/SAVE_RESTORE/CALL, laws 4/6/7 -- self-managed windows and the frame dance) is excluded by kind; GOTO_DEFERRED is a control sink (abandoned depth by design).  SCRIP_BB_ONLY/SCRIP_BB_SKIP = the per-BB dynamic arming Lon asked for (nid csv), the crawl's bisect instrument. */
    g_emit.op_pair_rejoin = (nd && nd->op == IR_MATCH_ALTERNATE) ? 1 : 0;   /* ALT-NARY REJOIN (s22j): set at the ONE choke every template invocation passes through, keyed on IR KIND (the sanctioned emitter dispatch axis), so x86_asm.h consumes a promoted scalar and stays encodings-only.  ALTERNATE ONLY -- see the op_pair_rejoin comment in emit.h for the measured reason ARBNO must keep its per-iteration pair claims. */
    g_emit.op_stmt_dyn = g_arbk16_stmt; if (nd) { if (nd->op == IR_STATEMENT_BEGIN) { g_arbk16_stmt = 0; g_emit.op_stmt_dyn = 0; } else if (nd->op == IR_STATEMENT_END || nd->op == IR_STATEMENT) g_arbk16_stmt = 0; }   /* ⭐ ARB-LON-K16: dynamic-extent verdict rides EVERY node from the armed ARBNO through the statement terminal (the terminal keeps it for its own release emission, then the latch clears) -- consumed by the FR-family operand renderers (___==stmt_base addressing, depth-immune) and the γ-jmp zgpop hook (mechanism-2 lea whack, BOTH exits incl. the ctx_restore fail edge); see emit.h op_stmt_dyn */
    g_emit.op_zres = 0; g_emit.op_zgpop = 0; g_emit.op_wsteal = 0; g_emit.op_ztail = 0; g_emit.op_zpat = 0; g_emit.op_zfc = 0; for (int _zi = 0; _zi < 6; _zi++) { g_emit.op_zread[_zi] = 0; g_emit.op_zkind[_zi] = -1; }   /* ZD-1: cleared at the choke UNCONDITIONALLY so glue and non-drive fills can never inherit a stale regime.  op_ztail (ZK-2): 0 for all non-IR_TO nodes; staged for IR_TO in codegen_flat_chain_body before the staging globals are consumed here.  op_zfc (L-3b, this session): 0 for all non-IR_MATCH_REPLACE nodes, mirrors op_zpat's clear. */
    g_emit.op_suspend_stmt_uclaim = 0;   /* ICN-FR-5: cleared per-node; staged value below (0 post-M-3 until ICN-FR re-plumbs the source) */
    if (g_zd_stage) { g_emit.op_suspend_stmt_uclaim = g_zd_suspend_uclaim; g_zd_suspend_uclaim = 0;   /* ICN-FR-5 twin applied at the choke, cleared so it never bleeds */    /* UCLAIM applied regardless of g_zd_arm; op_zw2 mechanism-2 flag applied beside op_zw */
        if (g_zd_arm) { g_emit.op_zres = 1; g_emit.op_fc_bytes = g_zd_k; g_emit.op_fc_base = -1; g_emit.op_zdepth = g_zd_k + g_zd_zunder;   /* ⭐ C-9 REPL-ZDEPTH (s35): IR_MATCH_REPLACE is K=0 (zd_k -- the splice owns no result cell, correct), so op_zdepth was 0.  But FRQ(off) on the RSP/FORTH arm is [rsp+off+op_zdepth], and at the splice the REPLACEMENT SUBTREE'S CELLS ARE STILL LIVE on the spine (r9 = lea of rv's own cell -- they CANNOT be popped first, which is why the CONST-WPOP failure-edge precedent does NOT apply here: that pops an ORPHANED cell on the omega edge, these are live on the success path).  With zdepth=0 all four frame reads (op_sa, op_sa+8, op_off, op_off+24 -- subject lo/hi and match start/end) were short by exactly the subtree footprint and read stale stack: MEASURED +16 for a literal replacement, +80 for `A '-' B`, i.e. NOT a constant but the subtree's own zeta depth.  g_zd_wpop is the planner's under-cells term (the same quantity line 842 hands to op_wpop) and is exactly that footprint.  ROQ (rip-relative name) and ZOPQ (raw-cell escape, ignores zdepth) were always correct -- that selectivity is the fingerprint that named this defect.  GATE WITNESS: P8_concat_repl -- any fix hardcoding 16 passes the literal case and FAILS the concat case. */   /* armed non-mech2: own K; mech2 armed: line-829 already set 8+Kc (correct for FR; ZRES/ZOPQ use raw-cell escapes and ignore zdepth) */ for (int _zi = 0; _zi < 6; _zi++) { g_emit.op_zread[_zi] = g_zd_read[_zi]; g_emit.op_zkind[_zi] = g_zd_kind[_zi]; } g_emit.op_ztail = g_zd_ztail; g_zd_ztail = 0; g_emit.op_zpat = g_zd_zpat; g_zd_zpat = 0; g_emit.op_zfc = g_zd_zfc; g_zd_zfc = 0;   /* ⭐ ZK-2 IR_TO BETA-RESUME FIX (this rung): op_ztail staged for IR_TO only (computed in codegen_flat_chain_body before this choke); 0 for all other kinds (cleared at line-835 unconditional clear; g_zd_ztail cleared here after consume so it never bleeds into the next node).  bb_to.cpp ZD arm uses ZLOC_B(o)=[rsp+op_ztail+16+o] at β-resume sites.  ONE AUTHORITY: g_zd_ztail is the ONLY writer, op_ztail is the ONLY reader -- this line and ZLOC_B in x86_asm.h are the two halves of the same law.  SN4/Prolog watermark: IR_TO is invisible to SN4 (byte-identical by construction). */ }
        g_emit.op_zgpop = (g_emit.flat_stmt_frame || (g_emit.flat_jmp_entry && g_emit.flat_pat)) ? 0 : g_zd_gpop; g_emit.op_wpop += g_zd_wpop; g_emit.op_wsteal = g_zd_wsteal; g_zd_stage = 0; }   /* ZGPOP-STF (s22a): ZERO HAND-COUNTED POPS on an armed graph -- the seed's law (seed/test_sno_stmt_frame_1.s: "fail edge = bracket cut; ZERO hand-counted pops", and the RETURN floater's "mov rsp,___ # reclaim EVERY statement/BB carve at any depth").  ZGPOP-BLOB (dc_sib_bt root cause): CLASS D blobs (flat_jmp_entry && flat_pat) also zero op_zgpop -- cells must be retained across γ for β re-entry via the 16B resume-record protocol (proc_PAT$N_γ pushes {res,___} at the deep frontier; proc_PAT$N_res → proc_PAT$N_β re-enters the node's β with rsp at the cell, depth-immune via pinned ___).  A γ-free (add rsp,K) before the push lifts rsp ABOVE the cell, proc_PAT$N_res over-corrects, and β reads 16B past the cell into the blob frame header.  The blob ω unwinds absolutely (lea rsp,[___+kt]) so no γ-free is needed; zeroing here is the ONE authority (same ZGPOP-STF precedent, same one-choke law).  Under STF a cross-statement gamma edge is REDIRECTED to the statement head's st_x leave stub (emit.cpp ~2327), which emits bb_glue_framed_leave = mov rsp,___; pop ___ -- a DEPTH-INDEPENDENT cut that already reclaims every ZD cell the statement suspended.  The staged add rsp,K in front of it is therefore dead: witness 023-shape `OUTPUT = A + B`, where n7_assign emitted `add rsp,48` immediately before `jmp main_stγ` whose first instruction is `mov rsp,___`.  Zeroed HERE at the planner choke rather than at the x86_asm.h:1943 emission site so BOTH consumers agree -- the conditional-gamma path (x86_asm.h:282) routes through the jcc invert synth on `op_zgpop > 0`, and gating only the emitter would fire that synth with no payload.  This is the ___ half of law 4 doing the job the hand-counted pop was standing in for: the value spine rides RSP, the release rides the bracket. */   /* ZD-1 (Lon s21x-v): the planner's numbers land here, once, at the ONE choke every template invocation passes through.  op_fc_bytes = the planner's K OVERRIDES the zw extent (the alpha carve is exactly the result cell), base stays -1 (the hook's op_zres term supplies the suspended discipline), op_zdepth mirrors K for any residual legacy spelling inside a converted body, op_wpop ADDS the planner's under-cells term to whatever the trampoline chase already accumulated. */
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
                       || nd->op == IR_MATCH_BREAK || nd->op == IR_MATCH_BREAKX || nd->op == IR_MATCH_DEFER || nd->op == IR_MATCH_ATP || nd->op == IR_MATCH_REPLACE || nd->op == IR_GOTO_DEFERRED
                       || nd->op == IR_NULLTEST_VAR || nd->op == IR_PROC_GEN || nd->op == IR_PROC_VALUE || ir_norm_call_kind(nd->op) == IR_CALL)
                    ? IR_LIT(nd).sval : (const char *)0;
    { extern int g_gva_active; extern int gva_index_of(const char *); int nm_op = (nd->op == IR_VAR || nd->op == IR_VAR_REF || nd->op == IR_ASSIGN);
      int defer_op = ((nd->op == IR_MATCH_DEFER) && IR_LIT(nd).sval && IR_LIT(nd).sval[0] != '*');
      g_emit.op_gva_k = (g_gva_active && (nm_op || defer_op) && IR_LIT(nd).sval && !graph_has_local(g_emit_cfg, IR_LIT(nd).sval)) ? gva_index_of(IR_LIT(nd).sval) : -1; }
    { extern int g_proc_direct_active; extern int proc_slot_of(const char *); extern int proc_direct_eligible(const char *); int nm_op = (ir_norm_call_kind(nd->op) == IR_CALL);
      g_emit.op_proc_k = (g_proc_direct_active && nm_op && IR_LIT(nd).sval && proc_direct_eligible(IR_LIT(nd).sval)) ? proc_slot_of(IR_LIT(nd).sval) : -1; }
    if (nd->op == IR_STATEMENT_BEGIN || nd->op == IR_STATEMENT_END || nd->op == IR_STATEMENT) g_emit.op_stno = (int32_t)IR_LIT(nd).ival;   /* MON-RE (SE-6 session): op_stno was promoted from IR_LIT(nd).ival for EVERY kind, but that field is the shared payload union -- a LEN count, a POS offset, ARBNO's ival=1, an ALT arity.  op_stno was therefore clobbered continuously during the walk, and the monitor LABEL tap baked whatever value happened to be resident when a STATEMENT_BEGIN emitted (nc.sno: statement 2 taps as stno=6).  Only the statement-bracket kinds carry a statement number, so only they may promote it; the tap's `op_stno > 0` guard was selecting accidents, not statements. */
    g_emit.op_ival = (ir_norm_call_kind(nd->op) == IR_CALL || nd->op == IR_PROC_GEN) ? (int64_t)nd->n_operands : (nd->op == IR_GOTO_DEFERRED ? (int64_t)nd->seal : IR_LIT(nd).ival);   /* ⭐ DEFINE-FOLD (s53): GOTO_DEFERRED's ival half of the union is CLOBBERED by the sval heap pointer; the fold-eligible mark rides nd->seal instead, staged here so the drive-phase value survives to the template. */
    if (nd->op == IR_SAVE_RESTORE) { int64_t r = (int64_t)zd_sr_role(nd); g_emit.op_ival = r; g_emit.op_sval = r ? (const char *)0 : IR_LIT(nd).sval; }   /* CALL2BB UNION-TAG: IR.h's sval/ival/dval are ONE union — roles 1-3 write ival and never sval; role 0 writes sval (the callee, an lp_strdup heap pointer) and never ival.  A heap pointer is never 1..3, so the arm read back IS the role; normalize HERE (the single dispatch point) so op_ival is a clean 0..3 and op_sval is name-or-null — templates never see the raw union.  The slice-1 code was accidentally correct (the pointer fell past the 1/2/3 role checks into the role-0 body); this makes the tag explicit and keeps the raw union read out of every other op_sval consumer. */
    g_emit.op_node_kind = (int)nd->op;
    g_emit.op_cap_anchor = 0;   /* FB-STMT per-node bit (Lon 2026-07-29) — twin lives in DRIVE_FILL; OS-2 anchor bit cleared per node, staged only at the capture-family dispatch */
    g_emit.op_dval = IR_LIT(nd).dval;
    g_emit.op_counter = 0;
    IR_t *op_a = (nd->n_operands > 0) ? nd->operands[0] : (IR_t *)0;
    g_emit.op_a_sval = op_a ? IR_LIT(op_a).sval : (const char *)0;
    g_emit.op_a_node_kind = op_a ? (int)ir_norm_call_kind(op_a->op) : -1;
    g_emit.op_a_slot = (op_a != (IR_t *)0) ? bb_slot_get(op_a) : -1;
    if (nd->op == IR_MATCH_VALUE && op_a) { int _z = nd_slot(op_a); if (_z >= 0) g_emit.op_a_slot = _z; }   /* PB-2 (2026-08-07c): TWO SLOT AUTHORITIES measured (pb_dbg4) — bb_slot_get(VAR)=128 while the VAR's own ZRES store landed at raw zls 96 (nd_slot); the consumer must read the PRODUCER's authority.  MATCH_VALUE only; every other op_a consumer keeps bb_slot_get. */
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
    case IR_BINOP_TEST:          bb_emit_x86(g_emit.op_zres ? bb_binop_relop_val() : bb_binop_relop()); return 0;   /* ⭐ ZK-2 BINOP_TEST ZD DISPATCH (s226): cells arm routes to bb_binop_relop_val() whose ZD top arm (op_zres && !op_num_real) reads lhs/rhs via ZOPQ(0,0/8)/ZOPQ(1,0/8) and writes ZRES(0/8), firing x86_gamma() on pass and x86_omega() on fail -- the correct {0,1}-generator shape per ARCH-ICON/ir_opfn.  Non-cells arm keeps bb_binop_relop() unchanged: it reads FRQ(op_sa/sb) which are flat-frame slots valid only when op_zres=0.  MEASURED ROOT CAUSE: loop-condition relops (i <= n, i >= 3, i < 0) never fired ω on the cells arm; their FRQ reads hit unrelated memory and the cmp produced a random truthy result, so every until/repeat-break/until-gen loop ran forever.  ONE AUTHORITY: this is the ONLY dispatch site for IR_BINOP_TEST. */
    case IR_BINOP_RELOP_VAL:     bb_emit_x86(bb_binop_relop_val());   return 0;
    case IR_BINOP:
        { extern int fc_vbinop_active(const IR_t *); extern long fc_vwpop(const IR_t *); long _w = fc_vwpop(nd); if (_w > 0 && fc_vbinop_active(nd) && !g_emit.op_zres) g_emit.op_wpop += (int)_w; }   /* ZB-VAL-5: generic-path DT_FAIL omega releases ALL live statement cells (binop carves nothing at alpha); gated on fc_vbinop_active directly -- op_fc_disp no longer the discriminator (ZD-3 legacy arm retirement) */
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
    case IR_MATCH_LEN:            { bb_prepare(nd); { const char * _sv = (nd->n_operands == 0 && (uintptr_t)(uint64_t)IR_LIT(nd).ival > (uintptr_t)0xFFFFU) ? IR_LIT(nd).sval : (const char *)0; g_emit.op_sval = (_sv && _sv[0] == '*') ? _sv : (const char *)0; } bb_emit_x86(bb_match_len()); } return 0;   /* SN4-PAT-1; D08 FIX: op_sval="*varname" for deferred-integer arm; guard: n_operands==0 (no pre-chain) AND union ival > 0xFFFF (heap ptr, not a LEN count) */
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
    case IR_MATCH_DEFER: { bb_prepare(nd); g_emit.op_seal = nd->seal; g_emit.op_off = drive_value_slot(nd); g_emit.op_defer_leaf_susp = g_emit_cfg ? fc_tail_defer_susp_g(g_emit_cfg, nd) : -1; bb_emit_x86(bb_match_defer()); } return 0;  /* SN4-PAT-FOLD deferred/stored pattern var; s137 OVER-SEAL: seal state written HERE (post-prologue, both drive paths) from IR_t.seal — op_off points at the defer.pad quad (the α rsp watermark) when FULL-sealed (==1); s142: seal==2 = write-once class, entry-cell only, no pad; s153: op_defer_leaf_susp arms the ZERO-GUARDED β for priced tail-candidate leaves (the ε-resume phantom-pad convention). ___-EARN s44 WIRE-SAVE: op_off is now ALWAYS staged (not gated on seal==1) — the zeta_storage.c ZLS grant for IR_MATCH_DEFER is unconditional (16B every node, seal==1 merely repurposes +0 as the fence watermark), so the quad is a safe, always-valid home; bb_match_defer.cpp's own wire-save arm additionally gates on seal!=1 (so it never fights the fence watermark for +0) and x86_fb_pinned() (so FRQ is depth-immune — see s43d's rsp-arm misread caveat) before actually using it. */
    case IR_MATCH_VALUE:          { bb_prepare(nd); bb_emit_x86(bb_match_value()); } return 0;  /* SN4 kill-manufactured-names: match operand[0]'s pattern value (op_a_slot), no name */
    case IR_MATCH_ARBNO:          { bb_prepare(nd); extern int fc_tail_arbno(const IR_t *, int *, int *, int *, int *); extern int fc_tail_ncap(const IR_t *); int _fpb = 0, _fpl = 0, _osb = 0, _hdr = 0;
                                    int _tailc = (x86_zc_frame() == ZC_FRAME_RSP && fc_tail_arbno(nd, &_fpb, &_fpl, &_osb, &_hdr)) ? 1 : 0;   /* ⭐ hoisted ONCE (const-pure analysis, no staging): the prelude below needs _fpb -- outer pending SAVE cells riding the frontier across this ARBNO.  K16's non-popping commits would interpose between such a cell and its COND's static-distance read (166_pat_arbno_cond_assign_commit witnesses this), so _fpb>0 declines to the carry-the-tail arm which exists precisely to relocate those cells. */
                                    int _k16r = 0; { static int _k16e = -1; if (_k16e < 0) { const char * _e1 = getenv("SCRIP_ARBNO_K16"); const char * _e2 = getenv("SCRIP_ARBNO_FRAMELESS"); _k16e = ((_e1 ? (atoi(_e1) != 0) : 1) && (_e2 ? (atoi(_e2) != 0) : 1)) ? 1 : 0; } if (_k16e && x86_port_mode() == ZC_PORT_FORTH && g_emit_cfg && nd->n_operands >= 3) { int _ni=-1; for(int _j=0;_j<g_emit_cfg->n;_j++) if(g_emit_cfg->all[_j]==nd){_ni=_j;break;} int _fr=0; if(_ni>=0) for(int _j=0;_j<g_emit_cfg->n&&!_fr;_j++){IR_t*_o=g_emit_cfg->all[_j]; if(_o&&_o!=nd&&_o->op==IR_MATCH_ARBNO&&_o->n_operands>=3){int _a=-1,_b=-1; for(int _k=0;_k<g_emit_cfg->n;_k++){if(g_emit_cfg->all[_k]==_o->operands[1])_a=_k;if(g_emit_cfg->all[_k]==_o->operands[2])_b=_k;} if(_a>_b){int _t=_a;_a=_b;_b=_t;} if(_a>=0&&_ni>=_a&&_ni<=_b)_fr=1;}} int _k0=1,_kk=0,_sq=1; { int _s0=-1,_s1=-1; for(int _j=0;_j<g_emit_cfg->n;_j++){if(g_emit_cfg->all[_j]==nd->operands[1])_s0=_j;if(g_emit_cfg->all[_j]==nd->operands[2])_s1=_j;} if(_s0>_s1){int _t=_s0;_s0=_s1;_s1=_t;} if(_s0<0){_k0=0;_sq=0;} else for(int _j=_s0;_j<=_s1;_j++){IR_t*_m=g_emit_cfg->all[_j]; if(_m&&_m!=nd){int _mo2=(int)_m->op; if(_mo2==IR_MATCH_ALTERNATE||_mo2==IR_MATCH_ARBNO||_mo2==IR_MATCH_FENCE1||_mo2==IR_MATCH_DEFER||_mo2==IR_MATCH_VALUE||_mo2==IR_CALL||_mo2==IR_CALL_VALUE||_mo2==IR_DISJUNCTION||_mo2==IR_MATCH_ABORT)_sq=0; int _mk=zd_k(_m); if(_mk!=0)_k0=0; _kk+=_mk;}} } int _osv=0,_dmb=-2,_dme=-2; { int _mb=-1,_hi=g_emit_cfg->n; for(int _j=_ni;_j>=0;_j--){if(g_emit_cfg->all[_j]&&g_emit_cfg->all[_j]->op==IR_MATCH_BEGIN){_mb=_j;break;}} if(_mb>=0) for(int _j=_mb+1;_j<g_emit_cfg->n;_j++){if(g_emit_cfg->all[_j]&&g_emit_cfg->all[_j]->op==IR_MATCH_BEGIN){_hi=_j;break;}} if(_mb>=0){ int _s0=-1,_s1=-1; for(int _j=0;_j<g_emit_cfg->n;_j++){if(g_emit_cfg->all[_j]==nd->operands[1])_s0=_j;if(g_emit_cfg->all[_j]==nd->operands[2])_s1=_j;} if(_s0>_s1){int _t=_s0;_s0=_s1;_s1=_t;} for(int _j=_mb+1;_j<_hi;_j++){ if(_j>=_s0&&_j<=_s1) continue; IR_t*_m=g_emit_cfg->all[_j]; if(_m&&_m->op==IR_MATCH_ASSIGN_SAVE)_osv=1; } } else _osv=1; _dmb=_mb; _dme=_hi; }   /* ⭐ OUTER-pending scan: a SAVE inside this match bracket but OUTSIDE the body span parks a cursor cell on the frontier ACROSS this ARBNO -- K16 commits would interpose between that cell and its COND static-distance read (166 witnesses).  Body-interior SAVEs travel WITH the instance and are exactly what the arm re-homes, so only the outer kind declines. */ if (_sq && !_k0 && _kk > 0 && !_fr && !_osv) { _k16r = 1; g_arbk16_stmt = 1; g_emit.op_arbno_framed = 0; g_emit.op_arbno_body_k0 = 0; g_emit.op_arbno_body_kk = _kk; }   /* framed (nested-in-another-ARBNO) context DECLINES: under a legacy chain outer, an inner activation must leave net-zero stack delta (the chain's per-iteration block arithmetic cannot represent a variable count of surviving committed cells) -- K16's non-popping commits violate that by design, so depth>0 keeps legacy until the outer chain itself is replaced (X03/X05 witness this law) */ { static int _ad=-1; if(_ad<0){const char*_e=getenv("SCRIP_ARBNO_DIAG"); _ad=(_e&&*_e=='1')?1:0;} if(_ad) fprintf(stderr,"[ARBNO-K16] framed=%d k0=%d sq=%d kk=%d osv=%d mb=%d me=%d route=%s\n", _fr, _k0, _sq, _kk, _osv, _dmb, _dme, _k16r?"FRAMELESS_K":"legacy"); } } }   /* ⭐ ARB-LON-K16 PRELUDE (this rung): the containment+ΣK scan runs BEFORE the tail claim so the sequence-only static-K body class (SAVE cells; kk = Σ zd_k over the span, THE ONE K AUTHORITY) routes to the re-homed frameless arm INSTEAD of fc_tail -- this is the ONE routing authority (env read HERE only; the template keys purely on the staged op_arbno_body_kk field, so the killswitches cannot half-flip).  SCRIP_ARBNO_K16=0 (or SCRIP_ARBNO_FRAMELESS=0) makes this prelude inert and every byte downstream identical to HEAD.  Container/transfer kinds in the span (path-dependent or unbounded frontier at body-γ) decline to the legacy routes verbatim. */
                                    if (!_k16r && 0 && _tailc) { g_emit.op_tail = 1;   /* ⛔ D-1 DELETE: carry-the-tail claim RETIRED -- the tail arm is gone, so its geometry must not stage; zls_arbno_geom below is now the sole ARBNO geometry authority */ g_emit.op_sb = _osb; g_emit.op_sa = _hdr; g_emit.op_tail_fpb = _fpb; g_emit.op_tail_fpl = _fpl; g_emit.op_tail_ncap = fc_tail_ncap(nd); g_emit.op_tail_seal = (nd->n_operands > 3 && nd->operands[3] == nd) ? 1 : 0; g_emit.op_tail_dfr = fc_tail_dfr(nd); { extern int fc_tail_fpr_rsp(const IR_t *); g_emit.op_tail_fpr_rsp = fc_tail_fpr_rsp(nd); } }
                                    else if (!_k16r) { int _mo = -1, _sp = 0; if (zls_arbno_geom(nd, &_mo, &_sp)) { g_emit.op_sa = _mo; int _chain = (x86_port_mode() == ZC_PORT_FORTH); g_emit.op_arbno_chain = _chain; g_emit.op_sb = (_sp + (_chain ? 24 : 16) + 15) & ~15; if (_chain) { g_emit.op_arbno_nzq = zls_arbno_zq(nd, g_emit.op_arbno_zq, 8); if (_chain && g_emit_cfg && nd->n_operands >= 3) { int _i0=-1,_i1=-1; for(int _j=0;_j<g_emit_cfg->n;_j++){if(g_emit_cfg->all[_j]==nd->operands[1])_i0=_j;if(g_emit_cfg->all[_j]==nd->operands[2])_i1=_j;} if(_i0>_i1){int _t=_i0;_i0=_i1;_i1=_t;} int _bha=0; for(int _j=_i0;_j<=_i1&&_j>=0&&!_bha;_j++) if(g_emit_cfg->all[_j]&&g_emit_cfg->all[_j]->op==IR_MATCH_ARBNO&&g_emit_cfg->all[_j]!=nd) _bha=1; g_emit.op_body_has_arbno=_bha; } }
                                           if (_chain && g_emit_cfg) { int _ni=-1; for(int _j=0;_j<g_emit_cfg->n;_j++) if(g_emit_cfg->all[_j]==nd){_ni=_j;break;} int _fr=0; if(_ni>=0) for(int _j=0;_j<g_emit_cfg->n&&!_fr;_j++){IR_t*_o=g_emit_cfg->all[_j]; if(_o&&_o!=nd&&_o->op==IR_MATCH_ARBNO&&_o->n_operands>=3){int _a=-1,_b=-1; for(int _k=0;_k<g_emit_cfg->n;_k++){if(g_emit_cfg->all[_k]==_o->operands[1])_a=_k;if(g_emit_cfg->all[_k]==_o->operands[2])_b=_k;} if(_a>_b){int _t=_a;_a=_b;_b=_t;} if(_a>=0&&_ni>=_a&&_ni<=_b)_fr=1;}} g_emit.op_arbno_framed=_fr; int _k0=1; { int _s0=-1,_s1=-1; for(int _j=0;_j<g_emit_cfg->n;_j++){if(g_emit_cfg->all[_j]==nd->operands[1])_s0=_j;if(g_emit_cfg->all[_j]==nd->operands[2])_s1=_j;} if(_s0>_s1){int _t=_s0;_s0=_s1;_s1=_t;} if(_s0<0)_k0=0; else for(int _j=_s0;_j<=_s1&&_k0;_j++){IR_t*_m=g_emit_cfg->all[_j]; if(_m&&_m!=nd&&zd_k(_m)!=0)_k0=0;} } g_emit.op_arbno_body_k0=_k0; }   { int _du=0; int _ds0=-1,_ds1=-1; for(int _j=0;_j<g_emit_cfg->n;_j++){if(g_emit_cfg->all[_j]==nd->operands[1])_ds0=_j;if(g_emit_cfg->all[_j]==nd->operands[2])_ds1=_j;} if(_ds0>_ds1){int _t=_ds0;_ds0=_ds1;_ds1=_t;} if(_ds0>=0) for(int _j=_ds0;_j<=_ds1;_j++){IR_t*_m=g_emit_cfg->all[_j]; if(_m&&_m!=nd&&(int)_m->op==IR_MATCH_DEFER&&!_m->pat_static)_du=1;} g_emit.op_arbno_body_defer_unsafe=_du; }   /* W-7 INTERIM GUARD (HOME-WIRES s38): SAME body span as op_arbno_body_k0 above, SAME containment idiom -- flags an IR_MATCH_DEFER member whose pat_static==0 (can transitively recurse, manual p.122's *X rule).  Independent of op_arbno_body_k0.  Consumed by bb_match_arbno() to decline the unconditional plain-frameless fallback with x86_bomb.  See emit.h field note for the full record. */   /* ⭐ ARBNO-FRAME containment dual of the op_body_has_arbno scan above: THIS node inside any OTHER ARBNO's [operands[1]..operands[2]] span = the nested/unbounded tier -> per-activation header frame (see emit.h field note; Lon directive 2026-08-06) */ } else { g_emit.op_sa = -1; g_emit.op_sb = -1; g_emit.op_arbno_chain = 0; } }
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
                                                if (_x->op == IR_MATCH_DEFER) { if (_dn || _x->seal != 2) _ok = 0; else _dn = _x; }
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
    case IR_MATCH_BEGIN:           { extern int fc_tail_head(const IR_t *); int _th = (x86_zc_frame() == ZC_FRAME_RSP && fc_tail_head(nd)); if (fc_head_fp(nd) >= 0 || _th) { g_emit.op_fc_wbytes = 80; g_emit.op_fc_base = g_emit.op_off; } if (_th) g_emit.op_tail = 1;    /* ⭐ ZW-RB-1: set op_mech2_kc from op_udout (=Kc); zero zpop -- mech2_whack handles cleanup; fc-hook wbytes skipped under op_zw2 -- mech-2 push ___ subsumes the CAS sentinel, no separate sub rsp,24 */  /* ⭐ M-2 HFC-WINDOW: op_fc_wbytes 24→80 -- covers all 80B of MATCH_BEGIN's ZLS fields (5 quads: cursor+zeta_mark+lls2_mark at +0..+23 were FC-live; +24..+79 = head.end+dcap_mark+incoming____+sigma_save+delta_save+Delta_save+capgen_save were FLAT-BYDESIGN under UCLAIM; M-3 deleted the UCLAIM head claim so FLAT-BYDESIGN now resolves to [___+off] in the ambient CRT frame (SIGSEGV). Widening the window to 80 FC-rebases ALL fields to [rsp + off - op_fc_base] inside the enlarged sub rsp,80 region. Window extension is safe: the FC rebase formula [rsp+off-base] gives the same bytes for +0..+23 as before (rebase=0 for those, unchanged); for +24..+79 the rebase steers into the newly-allocated 80B stack region. ONE SIDE EFFECT: x86_zclaim in the template must grow to 80B to match (see bb_match_begin.cpp hfc arm). */ bb_emit_x86(bb_match_begin()); } return 0;                  /* SN4-PAT-2 + ZB-FC-3d self-cell WINDOW + R12-EXIT-1 tail statements self-push too (bracket source) */
    case IR_MATCH_END:        { IR_t * _hd = nd->n_operands > 0 ? nd->operands[0] : (IR_t *)0; extern int fc_tail_release(const IR_t *, int *); int _br = -1;
                                    { int _tr = (x86_zc_frame() == ZC_FRAME_RSP && _hd) ? fc_tail_release(_hd, &_br) : 0; int _fp = _hd ? fc_head_fp(_hd) : -999;
                                      if (getenv("SCRIP_FCDISP_DIAG")) fprintf(stderr, "[FCDISP] tail_release=%d br=%d head_fp=%d\n", _tr, _br, _fp); }   /* L-3b STEP-5 DIAGNOSTIC (this session): env-gated, additive-only -- inline at the exact branch point, eliminates every remaining misattribution risk after three prior hand-traced theories (dead fc_head_fp register table / stale carryover / nested dispatch) each needed correction under direct measurement. Zero behavior change: this block only calls the SAME functions the real code calls one line below, purely for printing; does not alter op_fc_disp/op_tail. */
                                    if (x86_zc_frame() == ZC_FRAME_RSP && _hd && fc_tail_release(_hd, &_br)) { g_emit.op_tail = 1; g_emit.op_fc_disp = _br; }
                                    else g_emit.op_fc_disp = _hd ? fc_head_fp(_hd) : -1;
                                    /* ⭐ M-2 MATCH_END FC WINDOW: mirror MATCH_BEGIN's 80B window so PATCTX restores (FRQ(op_off+48..72)) FC-rebase to the same [rsp+N] slots the saves wrote. op_fc_wbytes=80, op_fc_base=head_loff (= g_emit.op_off set by DRIVE_FILL below from drive_value_slot(hd)). Set before bb_emit_x86 so the template's FRQ calls see the window.  hfc() = op_fc_wbytes>0: fires when MATCH_BEGIN used the hfc arm -- fc_head_fp>=0 OR fc_tail_head (ARBNO path). M-2 BUG-4: the fc_tail_head arm was missing here; MATCH_BEGIN armed hfc (sub rsp,80, saves at [rsp+48..72]) but MATCH_END fell to non-hfc ([___+96..120]) -- save/restore mismatch, crash. Mirror MATCH_BEGIN's condition exactly. */
                                    { extern int fc_tail_head(const IR_t *); if (_hd && (fc_head_fp(_hd) >= 0 || fc_tail_head(_hd)) && x86_zc_frame() == ZC_FRAME_RSP) { g_emit.op_fc_wbytes = 80; g_emit.op_fc_base = g_emit.op_off; } }
                                    bb_emit_x86(bb_match_end()); } return 0;               /* BB-OWNED-ζ statement-scope pivot + ZB-FC-3d cross-box cell reads + R12-EXIT-1 tail: bracket read off the TOP ELEMENT then unwind */
    case IR_MATCH_REPLACE:        { bb_emit_x86(bb_match_replace()); } return 0;               /* SN4-REPL stages 4/5 splice */
    case IR_MATCH_ASSIGN_COND:    { bb_prepare(nd); g_emit.op_fc_disp = fc_cond_fp(nd); g_emit.op_cap_anchor = cap_anchor_of(nd); if (getenv("SCRIP_CAP_DIAG")) fprintf(stderr, "[CAP] COND nd=%p fc_disp=%d port=%d zres=%d zread0=%d anchor=%d\n", (void*)nd, g_emit.op_fc_disp, (int)x86_port_mode(), g_emit.op_zres, g_emit.op_zread[0], g_emit.op_cap_anchor); bb_emit_x86(bb_match_capture()); } return 0;               /* SN4-PAT-2 + ZB-FC-3c cross-box read grant; ⭐ OS-2·SLICE-1: the D-FAMILY _fb/_adj defer-blob compensation blocks DELETED WHOLE per the land-ready spec (GOAL-SN4-ZETA-MECH) -- emit-time distance prediction across a dynamic extent, malformed per DYNAMIC-DEPTH (rt_fn_frame_bytes_known cannot see runtime-minted blob carves or the m4 defer continuation push) and unreachable for *VAR; the anchor arm reads SAVE's slot through THE ONE OFFSET FUNCTION instead, depth-immune on the pinned ___ arm. */
    case IR_MATCH_ASSIGN_IMM:     { bb_prepare(nd); g_emit.op_fc_disp = fc_cond_fp(nd); g_emit.op_cap_anchor = cap_anchor_of(nd); if (getenv("SCRIP_CAP_DIAG")) fprintf(stderr, "[CAP] IMM nd=%p fc_disp=%d anchor=%d off=%d pin=%d refine=%d deep=%d pat=%d\n", (void*)nd, g_emit.op_fc_disp, g_emit.op_cap_anchor, g_emit.op_off, emit_jmp_pin_legacy(), g_emit.flat_fb_refine, g_emit.flat_deep_arrival, g_emit.flat_pat); bb_emit_x86(bb_match_capture()); } return 0;               /* $ immediate capture + ZB-FC-3c (identical mechanism, phase 2); ⭐ OS-2·SLICE-1: D-FAMILY _fb/_adj block DELETED WHOLE (mirrors COND -- emit-time distance prediction across a dynamic extent); anchor arm reads SAVE's slot via THE ONE OFFSET FUNCTION. */
    case IR_MATCH_ASSIGN_SAVE:    { bb_prepare(nd); g_emit.op_cap_anchor = cap_anchor_of(nd); { long fck; if (fc_geom(nd, &fck)) { g_emit.op_fc_bytes = fck; g_emit.op_fc_base = g_emit.op_cap_anchor ? -1 : g_emit.op_off; } } { extern int fc_save_active(const IR_t *); if (getenv("SCRIP_CAP_DIAG")) fprintf(stderr, "[CAP] SAVE nd=%p save_active=%d fc_bytes=%ld port=%d anchor=%d\n", (void*)nd, fc_save_active(nd), (long)g_emit.op_fc_bytes, (int)x86_port_mode(), g_emit.op_cap_anchor); } bb_emit_x86(bb_match_capture()); } return 0;               /* SN4-PAT-3h phase-0 SAVE + ZB-FC-3c fixed-cell grant (δ at cell+0); ⭐ OS-2·SLICE-1: anchor bit staged from THE ONE ROUTING AUTHORITY (producer symmetric with COND/IMM by construction); op_fc_bytes stays staged so the α hook's sub rsp,16 and every neighbor's fp prefix math are unchanged, but op_fc_base is NULLED under anchor -- x86_fc_hit keys regime 2 on base>=0, and leaving it armed captured the anchor arm's FR(op_off) into the legacy rsp cell spelling (producer/consumer desync, measured); the dead 16B carve is slice-tolerated, δ lives in the node's own ZLS slot via FR(op_off) instead. */
    case IR_MATCH_ALTERNATE:      { bb_emit_x86(bb_match_alternate()); } return 0;                   /* SN4-PAT-3h ALT; ALT-FLAT s202 -- zero-cell box, address-dispatch template, no fc staging */
    case IR_MATCH_FENCE1:          { bb_prepare(nd); bb_emit_x86(bb_match_fence1()); } return 0;     /* SYNC-POINT ζ RELEASE sync point 2: NO fc_geom BY DESIGN — the watermark quad must stay [___+off] (depth-immune) because the σ glue reads it at the dynamic post-P depth */
    case IR_MATCH_ABORT:           { bb_emit_x86(bb_match_abort()); } return 0;      /* ABORT-NODE (s193): first live constructor is lower_snobol4.c TT_ABORT (was bare IR_GOTO); template pre-existed, flat_trivial_beta already anticipated the kind */
    case IR_SCAN_SEQUENCE:        { bb_emit_x86(bb_scan_sequence()); } return 0;   /* Icon scan concat: value = slice into own slot */
    case IR_SCAN_ALTERNATE:       { bb_emit_x86(bb_scan_alternate()); } return 0;  /* Icon scan alternation: bb_match_alternate wiring, value = slice into own slot */
    case IR_TO_BY:                { bb_prepare(nd); bb_emit_x86(bb_to_by()); } return 0;
    case IR_MAKE_LIST:            bb_emit_x86(bb_make_list());      return 0;
    case IR_CONJUNCTION:                 bb_emit_x86(bb_conjunction());           return 0;
    case IR_GOTO:                 { static int _mon = -1; if (_mon < 0) _mon = (getenv("MONITOR_BIN") && getenv("MONITOR_GOTO_TAP")) ? 1 : 0; extern void emit_mon_label_tap(int32_t); if (_mon && g_emit.op_stno > 0) emit_mon_label_tap(g_emit.op_stno); } bb_emit_x86(bb_goto());           return 0;   /* MON-RE (SE-6 session): the GOTO tap emits a LABEL at PURE WIRING nodes, which has no counterpart in SPITBOL's wire (SPITBOL labels statement boundaries only), so every program -- passing ones included -- diverged within 3 steps and the monitor could not bracket ANY bug.  IR_STATEMENT_BEGIN below is the true statement-boundary anchor and stays unconditional.  Set MONITOR_GOTO_TAP=1 to restore the old dense-tap behaviour. */
    case IR_GOTO_DEFERRED:             bb_emit_x86(bb_goto_dyn());       return 0;               /* EVAL/CODE runtime label transfer */
    case IR_FUNC_ACTIVATE: {   /* AB-1: fill op_sval=fname, op_ival=nsave; operands carry GVA indices of {fname,formals,locals} minted by the lowerer */
        if (nd->n_operands == 0) { g_emit.op_sval = IR_LIT(nd).sval;   /* ROLE DISCRIMINATOR = n_operands: IR_LIT is a UNION (sval clobbers ival — measured this session: the ival==2 guard sank every bind into drive_unowned); a role-1 block ALWAYS carries >=1 operand (fname), the bind carries none. */ extern std::string bb_ab_bind(); bb_emit_x86(bb_ab_bind()); return 0; }   /* AB-3a: role 2 = the DEFINE residual bind, a live main-chain box (anchor->bind->sJ); role 1 (blocks) rides g->ab_nodes[] via bb_ab_emit_nodes and never reaches this dispatch in-chain. */
        g_emit.op_sval = IR_LIT(nd).sval;
        g_emit.op_ival = (long)nd->n_operands;
        g_emit.op_ab_nformals = nd->seal;   /* AB-3b: nformals stored in seal by lowerer (seal is IR_MATCH_DEFER-specific, always 0 on IR_FUNC_ACTIVATE nodes) */
        { extern int g_gva_active; int _ab_n = (int)nd->n_operands; drive_arg_slots_reserve(_ab_n);   /* 599601e heap-backed op_arg_slot; old fixed-array sizeof idiom = 8/4 = silent cap 2 — same rot as the posthook site, fixed same commit */
          for (int _k = 0; _k < _ab_n; _k++) { const char * _nm = nd->operands[_k] ? IR_LIT(nd->operands[_k]).sval : (const char *)0; g_emit.op_arg_slot[_k] = (_nm && g_gva_active) ? gva_index_of(_nm) : -1; } g_emit.op_arg_slot_n = _ab_n; }   /* GVA indices for save-set {fname,formals,locals}: operands carry names as IR_LIT_STRING; gva_index_of resolves at emit time after GVA is built */
        extern std::string bb_func_activate();
        bb_emit_x86(bb_func_activate());
    } return 0;   /* LADDER AB (2026-08-08): per-DEFINE activation block.  α = ___-framed entry + save-set save + arg install + body-entry jmp.  β = return landing (type-code r15b from floaters) + restore + leave + jmp wire.  op_arg_slot[k] = GVA index of save-set member k (from operand->extra per the gva_index_of path). */
    case IR_SAVE_RESTORE:              { extern int fc_call_active(const IR_t *); if (g_emit.op_ival == 4 && nd->n_operands >= 2 && nd->operands[0] && nd->operands[1]) { g_emit.op_sval = IR_LIT(nd->operands[0]).sval; g_emit.lbl_t0 = IR_LIT(nd->operands[1]).sval; }   /* TINY-REAL s58 role-4 SHIM: operands = {fname, entry-label-name} as IR_LIT_STRING nodes (gather-then-deposit per the FACT RULE; ival=4 aliases sval so the names ride operands) */ if (g_emit.op_ival == 3 && nd->γ.node && IR_LIT(nd->γ.node).sval) { const char *_e = IR_LIT(nd->γ.node).sval; g_emit.op_sval = strncmp(_e, "LBL__", 5) ? _e : _e + 5; }   /* TINY-SITE s57: role-3 shim needs fname; γ = the stub's goto_deferred carrying entry_label "LBL__<FN>" — gather here per the FACT RULE (templates never read node neighbors) */ if (g_emit.op_ival == 0 && nd->γ.node && fc_call_active(nd->γ.node) && nd->n_operands == 1 && nd->operands[0]) { extern int zls_off(const IR_t *); int _as = bb_slot_get(nd->operands[0]); if (_as < 0) _as = zls_off(nd->operands[0]); if (_as >= 0) { g_emit.op_fc_wbytes = 16; g_emit.op_fc_base = _as; } } bb_emit_x86(bb_save_restore()); } return 0;               /* SN4-FLAT-PROC linkage: roles normalized at the UNION-TAG block above (0 CALL2BB site / 1 RETURN / 2 FRETURN / 3 wire-adopt).  CALL2BB 3b: when the gamma-paired call is fc-registered, sr0 gets the ARG window (base = the one arg's quad) so its existing FRQB(slot, sb) staging read self-rebases to [rsp + sb] -- the arg CELL above sr0's own save block -- with ZERO sr0 template edits (the s21x-h "fc arm inside FRQB" window-readiness, now armed); driver-side neighbor resolution is the sanctioned gather-then-deposit pattern. */
    case IR_STATEMENT_BEGIN:      { static int _mon = -1; if (_mon < 0) _mon = getenv("MONITOR_BIN") ? 1 : 0; extern int g_mon_max_stno; g_emit.op_mon_stmt_tap = (_mon && g_emit.op_stno > 0) ? 1 : 0; if (g_emit.op_mon_stmt_tap && g_emit.op_stno > g_mon_max_stno) g_mon_max_stno = g_emit.op_stno; g_emit.op_fc_bytes = 0; bb_emit_x86(bb_statement()); g_emit.op_mon_stmt_tap = 0; } return 0;   /* s26 (Lon): statement HEAD bracket -- alpha label = attribution range start; frame arm dormant (K=0 relay) until the FINDING-2026-08-02h STF rung; mon tap mirrors IR_GOTO's so the 2-way monitor syncs on true statement boundaries. */
    case IR_STATEMENT_END:
    case IR_STATEMENT:            { g_emit.op_fc_bytes = 0; bb_emit_x86(bb_statement()); } return 0;   /* ZW-5 (O-1 LIGHTING, s23q): op_zgpop is NOT set here -- the staging choke (grep \"ZD-1: cleared at the choke UNCONDITIONALLY\") is the ONE authority and supplies it from zd_plan's zd_gp[] via g_zd_gpop.  The prior `op_zgpop = nd->ival` was the s22k SECOND-SPELLING disease: nd->ival = lc_build default 0 (lower never stamps it -- K_total comes from the planner, not lower), so the direct read silently overwrote the planner's K_total with 0 and suppressed the release on every declined (UCLAIM) run.  op_fc_bytes=0 stays: K=0 is correct (zd_k), the statement box yields no cell. */
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
    case IR_GALT:                 { g_emit.op_off = 0; bb_emit_x86(bb_rk_galt()); }                 return 0;   /* RK-GRAM-3d grammar alternation: delta-save slot at [___+0]; lbl_t1 pre-resolved in pre-pass above */
    case IR_RETURN: {
        IR_t *rv = (nd->n_operands > 0 && nd->operands[0]) ? nd->operands[0] : (IR_t *)0;
        g_emit.op_sa = rv ? bb_slot_get(rv) : -1; g_emit.op_dval = IR_LIT(nd).dval;
        /* GENP-SPINE (s92): `return e` from a generator delivers the value through the RETAINING γ epilogue (frame + resume record stay live on the spine), so a subsequent β resume MUST route to failure —
         * Icon semantics: resumption of a returned procedure fails.  Arm the poison: bb_return stores lbl_ω's address into the resume slot pre-γ, so res-landing dispatch jmp [___+slot] takes the ABSOLUTE
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
        { extern int fc_vbinop_active(const IR_t *); extern long fc_vwpop(const IR_t *); long _w = fc_vwpop(nd); if (_w > 0 && fc_vbinop_active(nd) && !g_emit.op_zres) g_emit.op_wpop += (int)_w; }   /* ZB-VAL-5/6b: unop carves nothing at alpha; op_wpop carries the live depth for DT_FAIL omega; gated on fc_vbinop_active directly (ZD-3 legacy arm retirement) */
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
    g_emit.op_fc_bytes = 0; g_emit.op_fc_base = -1; g_emit.x86_fc_synth = 240; g_emit.op_fc_fpmax = -1; g_emit.op_fc_disp = -1; g_emit.op_fc_wbytes = 0; g_emit.op_arbno_chain = 0; g_emit.op_arbno_nzq = 0; g_emit.op_tail = 0; g_emit.op_tail_fpb = 0; g_emit.op_tail_fpl = 0; g_emit.op_tail_seal = 0; g_emit.op_tail_ncap = 0; g_emit.op_arbno_dt = 0; g_emit.op_arbno_dt_susp = 0; g_emit.op_defer_leaf_susp = -1; g_emit.op_tail_dfr = 0; g_emit.op_tail_fpr_rsp = 0; g_emit.op_body_has_arbno = 0; g_emit.op_arbno_framed = 0; g_emit.op_arbno_body_k0 = 0; g_emit.op_arbno_body_kk = 0; g_emit.op_arbno_body_defer_unsafe = 0; g_emit.op_cap_anchor = 0; \
    /* ⛔⭐ CARVE-DATA-ERAD (s23a, Lon 2026-08-01 "delete the ENTIRE data structure that stores the whole-graph data ... it is GONE NOW"): the fc_leaf_disp fill is DELETED -- op_flat_disp is permanently 0, the whole-graph displacement table has NO consumer, and the +op_flat_disp terms downstream are arithmetically dead pending janitorial code deletion.  The ⛔ step-2 falsification's 3-program cost is accepted by the directive. */ \
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
static int fc_alt_active(const IR_t *nd) { extern int fc_cells_active(void); return nd && nd->op == IR_MATCH_ALTERNATE && fc_cells_active() && fc_alt_fpmax(nd) >= 0; }   /* RBX census (FINDING-2026-08-12p/v): producer fc_alt_register (lower_snobol4.c:1677) is port-BLIND -- zero port checks in that file, gated only on fc_linear/fc_walk_range.  Widened from bare ZC_PORT_FORTH to match.  Measured BY SET byte-identical both ports (FORTH 84/122 0/0, HEAP 36/122 0/0) -- currently zero corpus impact, a real fix for a real mismatch. */
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

    DRIVE_FILL(nd, lbls[i], node_γ, node_ω, betas[i]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void drive_arg_slots_reserve(int n) {   /* promoted non-static for bb_ab_emit_nodes (bb_func_activate.cpp) — the bb_ab_fn_cell_ptr precedent; ONE allocator authority, declared emit.h */
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
        else if (vn) { int voff = bb_varslot_peek(vn); g_emit.op_sa = voff; g_emit.op_off = (voff != -1) ? drive_value_slot(nd) : -1; }   /* ICN-PROC-FRAME (s211): voff may be negative for ___-relative lexical locals; -1 is the unresolved sentinel.  op_off is the result cell (always needed when voff resolved). PL-FR-2: changed >= 0 to != -1 so negative ___-relative zframe slots are recognized as valid (not absent). */
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
          if (voff == -1) { fprintf(stderr, "[TE-4] IR_ASSIGN local '%s' has no LOWER-granted varslot — grant it in ir_drive_slot_assign (scrip_ir.c), never allocate in the emitter\n", vn); abort(); }   /* ICN-FR-3: -1 is the ir_varslot_of absent sentinel; NEGATIVE offsets other than -1 are valid ___-relative frame-local addresses ([___-(i+1)*16] for zframe graphs) and must NOT abort.  The prior `voff < 0` check was written when all varslots were non-negative ZLS offsets; zframe locals use negative offsets by design (matching rt_icn_zframe_args_install layout). */
          g_emit.op_sb = voff; }
        g_emit.op_off = drive_value_slot(nd); DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_CALL: case IR_CALL_BUILTIN: case IR_CALL_BUILTIN_GEN: case IR_CALL_PROC_STAGED: case IR_PROC_GEN:
    case IR_CALL_BUILTIN_ICON: case IR_CALL_BUILTIN_SNOBOL4: case IR_CALL_BUILTIN_PROLOG: {
        int na = nd->n_operands; drive_arg_slots_reserve(na);
        for (int i = 0; i < na; i++) { IR_t * a = ir_call_arg(nd, i); g_emit.op_arg_slot[i] = nd_slot(a); }
        g_emit.op_arg_slot_n = na; g_emit.op_write_route = bb_call_write_route(nd);
        { extern int bb_scc_handoff_consume(const void *, const char *); g_emit.op_c2 = bb_scc_handoff_consume((const void *)nd, IR_LIT(nd).sval); }   /* CALL2BB: 1 = this exact node's sr0 armed the prefix; -1 = armed under a different callee name (bomb in the template); 0 = no partner */
        { IR_t *_gt = nd->γ.node; int _gg = 0; while (_gt && _gt->op == IR_GOTO && _gg++ < 128) _gt = _gt->γ.node; IR_t *_ot = nd->ω.node; int _og = 0; while (_ot && _ot->op == IR_GOTO && _og++ < 128) _ot = _ot->γ.node; g_emit.op_sb = (g_emit_cfg && g_emit_cfg->icn_cells_graph && _gt && _ot && _gt == _ot && (unsigned char)nd->ω.sz[0] == 0xce && (unsigned char)nd->ω.sz[1] == 0xb2) ? 1 : 0; }   /* ZK-3 EVERY-BODY FLAG: op_sb=1 signals the ZD template arm to route gamma to beta_trampoline rather than x86_gamma(). In ir_a_Every both body ports route back to the generator beta; on the cells arm CALL_BUILTIN gamma=IR_SUCCEED and omega is a beta port (0xce 0xb2 tag, same raw bytes as port_sz_beta which is static below this call site). op_sb=1 tells the template to emit beta_trampoline on both exits. GATED icn_cells_graph+IR_SUCCEED+beta-omega tag: SN4/Prolog/top-level CALL leaves are byte-identical (op_sb stays 0). ONE AUTHORITY: only this dispatch and the ZD arm in bb_call_fn.cpp read op_sb for CALL_BUILTIN_ICON. */
        DRIVE_PAIR_RESET(); DRIVE_PAIR_DEF_JMP(lbl_β, lbl_ω); DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_MATCH_BEGIN: {
        IR_t *subj = nd->n_operands > 0 ? nd->operands[0] : (IR_t *)0;
        g_emit.op_sa = subj ? drive_value_slot(subj) : -1;
        { extern int fc_vread_fp(const IR_t *); g_emit.op_subj_cell = (x86_zc_frame() == ZC_FRAME_RSP && x86_port_mode() == ZC_PORT_FORTH && subj && fc_vread_fp(nd) >= 0) ? 1 : 0; }   /* SUBJECT-CELL rung (a): the head consumes the subject DESCR from the FORTH cell the registered producer chain left at TOS; fvr is node-keyed so sharing the ASSIGN registry cannot collide (the line-866 consumer only ever sees IR_ASSIGN nodes) */
        g_emit.op_off = drive_value_slot(nd);
        DRIVE_PAIR_RESET();
        if (g_emit.lbl_t0_p) { for (int _q = 0; _q < 3; _q++) { int _z = g_emit.xa_bb_emit_pair_n++; g_emit.xa_bb_emit_pair_define[_z] = NULL; g_emit.xa_bb_emit_pair_jmp[_z] = NULL; } int _z = g_emit.xa_bb_emit_pair_n++; g_emit.xa_bb_emit_pair_define[_z] = g_emit.lbl_t0_p; g_emit.xa_bb_emit_pair_jmp[_z] = NULL; g_emit.lbl_t0_p = NULL; g_emit.lbl_t0 = NULL; }   /* SEAL-UNWIND: pair[3] = BEGIN's na_f, def'd by the template at its L(1) anchor-exhaust unwind; slots 0-2 empty.  Consumed once, then cleared. */
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_MATCH_END: {
        /* BB-OWNED-ζ statement-scope pivot: operand[0] = the statement's IR_MATCH_BEGIN node (set by
         * sno_lower_match's ir_operand_push); read ITS slot, same convention as IR_MATCH_ARBNO's non-owner
         * phases reading role 0's slot via operand[0] two cases below. */
        IR_t *hd = nd->n_operands > 0 ? nd->operands[0] : (IR_t *)0;
        g_emit.op_off = hd ? drive_value_slot(hd) : -1;
        if (getenv("SCRIP_EDRIVE_END_DIAG")) fprintf(stderr, "[EDRIVE-END] op_off=%d op_fc_disp(pre)=%d op_tail(pre)=%d\n", g_emit.op_off, g_emit.op_fc_disp, g_emit.op_tail);   /* L-3b STEP-4 DIAGNOSTIC (this session): env-gated, additive-only -- checks whether emit_drive's IR_MATCH_END case (which never sets op_fc_disp) sees a STALE value here left over from an earlier node, or -1 from the per-node reset -- disambiguates two competing hand-traced theories from this session without further speculation. Zero behavior change. */
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_GOTO_DEFERRED: {
        g_emit.op_sval = IR_LIT(nd).sval; g_emit.op_ival = nd->seal; DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;   /* ⭐ DEFINE-FOLD (s53): op_ival carries the fold-eligible seal (1 = constant DEFINE entry label, bb_goto_dyn emits the direct chain transfer) */
    }
    case IR_SAVE_RESTORE: {
        { int64_t v = IR_LIT(nd).ival; int64_t r = (v == 1 || v == 2 || v == 3 || v == 4) ? v : 0; g_emit.op_ival = r; g_emit.op_sval = r ? (const char *)0 : IR_LIT(nd).sval; }   /* UNION-TAG normalization, twin of the dispatch-point block (which runs later and would otherwise re-poison these from the raw union) */
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
    case IR_MATCH_FENCE1: {   /* s137 OVER-SEAL: ival==0 = the FENCE0 interior sync box — operand-free, PURE 4-port (α whacks to the ___ floor, β abandons), so it rides the GENERIC drive (op_ival promoted
                               * from IR_LIT at the prologue above; the granted quad is unused by the ival=0 template arm).  The operand form (FENCE(P), ival==1) keeps the flat-path-only contract below. */
        if (nd->n_operands == 0) { g_emit.op_off = drive_value_slot(nd); DRIVE_PAIR_RESET(); DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break; }
        drive_unowned(nd);       /* replaced below by flat_drive path — the n-ary constructs are driven from codegen_flat_chain_body where the label arrays live */
        break;
    }
    case IR_MATCH_ABORT: {   /* ABORT-NODE (s193): operand-free PURE 4-port kill box (α = jmp ω, β trampoline → ω) — labels only, no slot, so op_off = -1 (the POS no-operand sentinel) and no drive_value_slot grant, unlike FENCE0 above whose granted quad is at least reserved by its template contract. */
        g_emit.op_off = -1; DRIVE_PAIR_RESET(); DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_MATCH_ALTERNATE: case IR_MATCH_ARBNO: case IR_SCAN_SEQUENCE: case IR_SCAN_ALTERNATE: {   /* SN4-NARY-ALT/-ARBNO + Icon scan nary: 2N operands = (entry_i, resume_i), N = n_operands/2 (ARBNO's [2] = geometry bracket, N=1); pairs 0..N-1 = entry α's, N..2N-1 = resume β's, 2N = success-glue def, 2N+1 = fail-glue def */
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
        g_emit.op_sa = a0 ? bb_slot_get(a0) : -1; if (a0 && g_emit.op_sa < 0) { extern int zls_off(const IR_t *); int _zsa = zls_off(a0); if (_zsa >= 0) { g_emit.op_sa = _zsa; g_emit.op_zres = 1; } } g_emit.op_off = -1;
        if (a0 && g_emit.op_sa < 0) { drive_unowned(nd); break; }
        { const char * _sv = (nd->n_operands == 0 && (uintptr_t)(uint64_t)IR_LIT(nd).ival > (uintptr_t)0xFFFFU) ? IR_LIT(nd).sval : (const char *)0; g_emit.op_sval = (_sv && _sv[0] == '*') ? _sv : NULL; }   /* D08 FIX: deferred-integer arm — sval="*varname" set by lower when TT_DEFER arg; guard against ival-union raw value (counts are ≤0xFFFF; heap ptrs are above) */
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
        if (!a0) { g_emit.op_sb = (int)IR_LIT(nd).ival; g_emit.op_sa = -1; }   /* CONST-AT-LOWER (Lon 2026-08-13): n_operands==0 == the count RIDES THE NODE (lower folded it, LEN's shape) — no chain cell exists, so no wpop and no slot */
        else if (a0->op == IR_LIT_INTEGER) { g_emit.op_sb = (int)IR_LIT(a0).ival; g_emit.op_sa = -1; }
        else { int sl = bb_slot_get(a0); if (sl < 0) { drive_unowned(nd); break; } g_emit.op_sa = sl; g_emit.op_sb = 0; }
        g_emit.x86_scratch_off = drive_value_slot(nd);
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_MATCH_POS: case IR_MATCH_RPOS: {
        IR_t * a0 = nd->n_operands > 0 ? nd->operands[0] : (IR_t *)0;
        g_emit.op_off = -1;
        if (!a0) { g_emit.op_sb = (int)IR_LIT(nd).ival; g_emit.op_sa = -1; }   /* CONST-AT-LOWER (Lon 2026-08-13): n_operands==0 == the position RIDES THE NODE (lower folded it, LEN's shape) — no chain cell, so NO CONST-WPOP: the 16B orphan that hack released never exists on this arm */
        else if (a0->op == IR_LIT_INTEGER) { g_emit.op_sb = (int)IR_LIT(a0).ival; g_emit.op_sa = -1; { extern int fc_frameless_fpr_rsp(const IR_t *); if (g_zd_arm && fc_frameless_fpr_rsp(a0)) g_emit.op_wpop += 16; } }   /* ⭐ ZD-5b POS/RPOS CONST-WPOP (s31/s32): const arg ZD-armed in FORTH mode → LIT_INT's 16B on rsp, never read via ZOPQ; orphaned on failure. fc_frameless_fpr_rsp = THE ONE AUTHORITY (zeta_storage.c OS-2·SLICE-ARBNO-FPR: ZC_PORT_FORTH && !fc_geom). g_zd_arm = per-run gate (LIT_INT armed this compile). op_wpop+=16 arms x86_fc_jcc_omega: je Lγ / add rsp,16 / jmp ω. N21 ARBNO right-spine witness. */
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
          if (voff == -1) { fprintf(stderr, "[TE-4] IR_REV_ASSIGN local '%s' has no LOWER-granted varslot — grant it in ir_drive_slot_assign (scrip_ir.c), never allocate in the emitter\n", vn); abort(); }   /* ICN-FR-3: -1 absent sentinel; negative offsets other than -1 are valid ___-relative frame-local addresses */
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
        if (ln[0] != '&') { int voff = bb_varslot_peek(ln); if (voff == -1) { fprintf(stderr, "[TE-4] IR_REV_SWAP lhs local '%s' has no LOWER-granted varslot — grant it in ir_drive_slot_assign (scrip_ir.c), never allocate in the emitter\n", ln); abort(); } g_emit.op_sb = voff; }   /* ICN-FR-3: == -1 absent sentinel */
        if (rn[0] != '&') { int voff = bb_varslot_peek(rn); if (voff == -1) { fprintf(stderr, "[TE-4] IR_REV_SWAP rhs local '%s' has no LOWER-granted varslot — grant it in ir_drive_slot_assign (scrip_ir.c), never allocate in the emitter\n", rn); abort(); } g_emit.op_sa = voff; }   /* ICN-FR-3: == -1 absent sentinel */
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
    case IR_FUNC_ACTIVATE:   /* AB-3a: role-2 bind in the live chain.  SECOND-DISPATCHER law (the 1560 comment, verbatim mechanism): emit_drive needs its own case or drive_unowned FATALs "op=15 has no template" while the walk case plainly exists — MEASURED on ab_recurse this session, same as ZW-5 SLICE 2 measured on probe1.  Shape = the SUCCEED/CUT trailer contract; the walk case's ival==2 arm supplies bb_ab_bind. */
        if (nd->n_operands != 0) { drive_unowned(nd); break; }   /* role discriminator = n_operands (IR_LIT union: sval clobbers ival — measured); blocks (>=1 operand) never enter the chain */
        DRIVE_PAIR_RESET(); DRIVE_PAIR_JMP(lbl_γ); DRIVE_PAIR_DEF_JMP(lbl_β, lbl_ω); DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;
    case IR_STATEMENT_BEGIN:
    case IR_STATEMENT_END:
    case IR_STATEMENT:
        g_emit.op_fc_bytes = 0;
        DRIVE_PAIR_RESET(); DRIVE_PAIR_JMP(lbl_γ); DRIVE_PAIR_DEF_JMP(lbl_β, lbl_ω); DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;   /* ⭐⭐ ZW-5 SLICE 2 (OMEGA O-1) — THE CASE SLICE 1 MISSED, FOUND BY RUNNING NOT READING.  Slice 1 reported "dispatch case live" and it IS live, but only in walk_bb_node_inner's switch (the m4 text walker); emit_drive is a SECOND dispatcher and a node it has no case for falls to drive_unowned, whose FATAL says "op=124 has no template in the universal driver" WHILE A CASE PLAINLY EXISTS -- the message's own footnote about the backtrace naming the real guard is what resolves the contradiction.  MEASURED: with SCRIP_ZW5=1 the very first minted box aborted the compile on probe1.sno before this case existed.  SHAPE: identical to IR_SUCCEED/IR_CUT above, which is exactly the trailer contract -- jmp gamma (the release rides that jmp's X86H hook), beta trampoline to omega.  ⛔ op_zgpop is DELIBERATELY NOT SET HERE: the staging choke (grep "ZD-1: cleared at the choke UNCONDITIONALLY") is the ONE authority and supplies it from the planner's zd_gp; the walk_bb_node_inner case's `op_zgpop = nd->ival` is a LATENT SECOND SPELLING (the s22k disease) that is inert only because lower never stamps ival -- it must be reconciled to the planner in the lighting commit, not carried forward. */
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
    case IR_GALT:
        DRIVE_FILL(nd, lbl_α, lbl_γ, lbl_ω, lbl_β); break;   /* RK-GRAM-3d: lbl_t1 pre-resolved in pre-pass; op_off=0 set at dispatch */
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
/* ⭐⭐⭐ U-1b UNWIND PRED WHITELIST (Lon HQ directive 2026-08-03c, "get every BB to FREE on OMEGA"): the kinds whose β may be the LANDING of an unwind roll.  THE LAW IS ALREADY IN THE TEMPLATE LAYER and
 * this predicate only names who may be jumped AT: 73 templates already close with x86_beta_trampoline() = `β: [hook: add rsp,K_own] jmp <resolved ω>`, and x86_asm.h's ONE ω arm
 * (`site==X86H_JMP && port==X86P_OMEGA && !op_wsteal` -> bb_glue_flat_leave) already frees own K on EVERY box's ω.  So a box's β IS its ω-continuation exactly as clause 2 states, already emitted, and the
 * ONLY reason the unwind could not use it was that flat_trivial_beta named four match kinds -- a whitelist minted for the ΣK POP FOLD, whose question ("may I INLINE this β away") is the mirror-image of
 * this one ("may I ROLL INTO this β").  Reusing it made U-1a's fire set provably empty (FINDING-2026-08-03 §3): plain-edge armed value-spine members ∩ four match kinds = ∅.  MEASURED at HEAD on the uw3
 * witness: n9_cmp_test's ω is `add rsp,16` (own K -- the law, already honored) THEN `add rsp,64` (the accumulated whack -- what clause 2 VOIDS) then a jmp straight to the :F target, skipping every
 * predecessor's β.  The four fold kinds stay listed: a trivial-β kind is a legal roll target too (its β is the same trampoline), so this is a strict WIDENING and the fold's own population is unchanged. */
static int flat_unwind_beta(const IR_t *nd) {
    switch (nd->op) {
    case IR_VAR: case IR_LIT_INTEGER: case IR_LIT_STRING: case IR_BINOP: case IR_UNOP: case IR_ASSIGN: case IR_ASSIGN_VAR:
    case IR_CMP_TEST: case IR_COERCE_NUMERIC: case IR_COERCE_STRING: case IR_COERCE_INTEGER: case IR_COERCE_REAL:
    case IR_DEREF: case IR_SUBSCRIPT: case IR_FIELD_VAR: return 1;
    case IR_MATCH_ALTERNATE: return 1;   /* ⭐ U-2 (2026-08-03): ALTERNATE is a generator — its β is `jmp [resume_ptr]` (bb_match_alternate.cpp:64-66), which re-enters the NEXT arm on backtrack.  Joining the unwind chain means a value-spine node AFTER an ALTERNATE rolls its fail edge into ALTERNATE.β rather than accumulating a depth whack.  This is clause 2 of the UNWIND LAW applied to the match family: each generator frees its OWN K (K=0 for ALTERNATE) and rolls to predecessor's β.  ALTERNATE.K==0 (zd_k returns 0), so the LOCAL DIFFERENCE pop for a node rolling here is its own (zd_k+zd_wp), which is the correct sole release.  β is ALWAYS LIVE (flat_beta_used_scan marks every operand-child of an IR_MATCH_ALTERNATE, line ~2140; x86_beta_trampoline fires for every armed member; bused[ALTERNATE] is set by ir_is_generator_kind or the operand-child scan).  ONE AUTHORITY — this case is the ONLY spelling of this decision per the s22k law.  Mirror sites at emit.cpp:~2381 (U-1b pre-pass) and ~2614 (drive loop) both call flat_unwind_beta() and inherit this widening atomically. */
    case IR_MATCH_ARBNO: return 1;   /* ⭐ U-2 (2026-08-03): ARBNO is a generator — its β is the element-push extension path (bb_match_arbno.cpp: sub rsp,op_sb + header init + jmp PAIR(0)), which extends the match by one more iteration on retry.  Same law as ALTERNATE: ARBNO.K==0 (zd_k returns 0), β is always live (ir_is_generator_kind or operand-child scan), and the LOCAL DIFFERENCE pop for a node rolling here is correct (ARBNO itself carries no value cell, so no over-release).  The FORTH-style stack discipline (RSP carries element frames; ___ pins the match floor) means ARBNO.β landing from the unwind chain arrives with RSP already restored to the pre-push frontier by the caller's own-K free — this is the O(1) frame-restore the HQ ruling names: mechanism-2 nested ___ frames handle indeterminable extent INSIDE the β extension, and the unwind chain handles the fail-side EXIT from any partially-extended match.  ONE AUTHORITY per s22k. */
    default: return flat_trivial_beta(nd);
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
static int port_sz_beta(const char * sz) { return sz[0] == (char)0xce && (unsigned char)sz[1] == 0xb2; }   /* R1 β-TAG SPLIT (s2026-08-06e diagnosis, landed this session): THE ONE SNIFF -- the five raw 0xce/0xb2 spellings (planner ZD-5b, used-scan, drive route, MOVE_LABEL, ZPOP-FOLD) collapse here per the s22k one-authority law.  The tag means exactly one thing at this level: "this edge lands on the target's β label" (label SELECTION, meaning (a)). */
/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int beta_is_stmt_land(const IR_t * tgt) { return tgt && tgt->op == IR_STATEMENT_BEGIN; }   /* R1 β-TAG SPLIT, meaning (b): a β-edge is a SCANNER RETRY BACK-EDGE (release-suppressing, in-match backtrack discipline) for every target EXCEPT IR_STATEMENT_BEGIN -- STATEMENT_BEGIN.β is a PURE LANDING PAD (06e Layer 1: by the UNWIND LAW every box freed its own K rolling home, rsp is at claim base on arrival), so edges into it keep NORMAL release behavior.  The suppression sites (planner oin, the UNWIND/ENDJMP/ZW5 steal guards) conjoin !beta_is_stmt_land(chased target); the label-routing sites stay on the raw tag.  At the pre-fB HEAD no β-edge targets STATEMENT_BEGIN, so this split is byte-identical until the lowerer's fB exhaust edge lands. */
/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int zd_wl_kind(IR_t * nd) {
    int op = (int)nd->op;
    extern int is_global(const char *);
    if (g_emit.zframe_graph && !(g_emit_cfg && (g_emit_cfg->icn_cells_graph || g_emit_cfg->pl_cells_graph))) {
        /* ICN-FR-2: zframe graphs pre-allocate flat_frame_bytes on entry (sub rsp,kt; mov ___,rsp); ZD spine-relative reads (ZOPQ = [rsp+op_zread[k]]) compute depth offsets WITHOUT accounting for this pre-allocation.  Result: n1's ZOPQ(0,0) reads from the FORTH spine cell [___-16] while n0 wrote to the ZLS frame slot [___+80] — 96 bytes apart.  Excluding zframe forces all nodes to the non-ZD (FRQ = [___+N]) path, which reads the frame slots correctly at any spine depth.  R-ICN-D: g_emit.zframe_graph=0 for all non-Icon graphs (SN4/Prolog/Raku/Pascal) so this return is never taken for them.  ZK-3 ADDITIVE: icn_cells_graph is the CELLS track (R-ZK-A routing law: mutually exclusive with zframe_graph at lower_icon.c — one graph is never in both arms); the cells arm uses FORTH-spine ZOPQ addressing which is correct regardless of the frame pre-allocation.  The conjunct lets the cells arm reach the per-kind whitelist below even when the graph's zframe_graph bit is 0 (cells arm sets icn_cells_graph, not zframe_graph).  PL-ZK-1b ADDITIVE: pl_cells_graph is the Prolog cells track; PL-FR-2 stamps zframe_graph=1 on all Prolog graphs (lower_prolog.c:1395), so both bits are simultaneously 1 when SCRIP_PL_CELLS=1 + SCRIP_PL_ZFRAME=1.  The pl_cells arm uses RSP-relative ZOPQ addressing (no ___ frame reads) and is correct under the zframe prologue — exempt it from the early-exit identically to icn_cells_graph.  R-PL-ZK-A mutual exclusion is now enforced HERE at the guard (not at lower_prolog.c, which cannot prevent PL-FR-2's wholesale zframe_graph stamp). */
        /* ICN-FR-3: IR_VAR and IR_ASSIGN that name frame-locals are EXEMPT from the blanket ZOPQ exclusion.  Those templates use FRQ(varslot) = [___+N] exclusively — they never touch the FORTH RSP spine (no ZOPQ reads, no op_zread staging) — so the depth-offset confound that motivated the early-exit does not apply.  zframe_graph guarantees the prologue has pinned ___ = activation base, so FRQ is depth-immune by construction (emit_jmp_pin____() is true for every zframe graph; x86_fb_pinned() follows).  The conjunct graph_has_local gates only LEXICAL locals (params + declared locals); globals and &-keywords are never admitted here (they are handled by the IR_VAR/IR_ASSIGN per-kind check below, which routes them to bb_var_global/bb_assign_global via the is_global branch and is also reached via this exemption).  SNOBOL4/Prolog/Raku/Pascal watermark: those lowerers never set zframe_graph, so this path is structurally invisible to them.  ONE AUTHORITY: only this site exempts zframe locals; the per-kind check at the bottom still performs the precise vn/graph_has_local/x86_fb_pinned predicate that decides the final return value for these kinds. */
        int op = (int)nd->op;
        if (op == IR_VAR || op == IR_ASSIGN) { extern int is_global(const char *); const char *vn = IR_LIT(nd).sval; if (vn && graph_has_local(g_emit_cfg, vn)) goto zframe_local_admitted; }
        return 0;
    }
    if (!(g_emit_cfg && (g_emit_cfg->icn_cells_graph || g_emit_cfg->pl_cells_graph))) return 1;   /* UNIVERSAL ARM (Lon s12 "turn it on for every single BB"): every FORTH-graph kind admitted; the per-kind arms below survive only as the icn/pl cells-track curation (ZK-5 globals guard etc.), which Lon exempted */
    zframe_local_admitted:;
    if (op == IR_LIT_INTEGER || op == IR_LIT_STRING || op == IR_LIT_REAL || op == IR_LIT_CHARSET) return 1;
    if (op == IR_CALL_PROC_STAGED && !(g_emit_cfg && g_emit_cfg->pl_cells_graph)) { static int _zk2cps = -1; if (_zk2cps < 0) { const char * e = getenv("SCRIP_ZD_ICN_CPS"); _zk2cps = (e && *e == '0') ? 0 : 1; } return (_zk2cps && g_emit_cfg && g_emit_cfg->icn_cells_graph) ? 1 : 0; }   /* ZK-2 ADDITIVE: IR_CALL_PROC_STAGED ICN arm -- admitted on icn_cells_graph.  PL-ZK-4b: guarded !pl_cells_graph so Prolog graphs fall through to the PL-ZK-1 arm at line 1974 (two arms for one op in an if-return chain means first match wins; without this guard the ICN arm returns 0 for Prolog and the PL arm is never reached).  SN4 watermark: icn_cells_graph=0 for all SN4 graphs byte-identical.  Killswitch SCRIP_ZD_ICN_CPS=0.  ONE AUTHORITY per s22k law. */
    if (op == IR_UNOP) { int v = (int)IR_LIT(nd).ival; return (v == TT_MNS || v == TT_PLS || v == TT_SIZE || v == TT_CSET_COMPL) ? 1 : 0; }
    if (op == IR_BINOP) { long long o = (long long)IR_LIT(nd).ival; return (o == BINOP_ADD || o == BINOP_SUB || o == BINOP_MUL || o == BINOP_DIV || o == BINOP_MOD || o == BINOP_POW || o == BINOP_CUNION || o == BINOP_CDIFF || o == BINOP_CINTER || o == BINOP_CONCAT) ? 1 : 0; }   /* ZD-2a: BINOP_CONCAT ADMITTED -- bb_binop_concat_slot grew its ZD arm this rung, and binop_cat maps BINOP_CONCAT to BINOP_CAT_CONCAT which dispatches to that ONE template unconditionally (emit.cpp 654/917), so the 017 falsification below cannot recur for this op; RELOP/XREP stay out until 2b/2c give them arms.  ORIGINAL NOTE, still binding for the remaining categories: EXPLICIT arith/cset ops only -- exactly the set bb_binop_arith's ZD arm serves.  IR_BINOP also carries CONCAT/XREP/RELOP categories that dispatch to templates WITHOUT a ZD arm (emit.cpp binop_cat switch); arming those staged op_zres for a template that ignores it, so the legacy body read flat slots the producers never wrote -- the 017_concat_two_strings signature of the first wide run. */
    if (op == IR_VAR || op == IR_ASSIGN) { const char * vn = IR_LIT(nd).sval; int _icn_cells = g_emit_cfg && g_emit_cfg->icn_cells_graph; return (vn && ((is_global(vn) && !graph_has_local(g_emit_cfg, vn) && !_icn_cells) || (graph_has_local(g_emit_cfg, vn) && x86_fb_pinned()) || (graph_has_local(g_emit_cfg, vn) && _icn_cells && x86_fb_pinned()))) ? 1 : 0; }   /* ZK-2 ADDITIVE: icn_cells_graph locals admitted (ZK-2, s212). ZK-5 ADDITIVE: !_icn_cells on the global conjunct -- on the cells arm globals must NOT be admitted to the ZD spine; they route to GVA/NV (process-lifetime storage, not control-flow-lifetime ζ per the ZK-5 charter). Without this guard the global conjunct armed IR_VAR/IR_ASSIGN(global) on the cells arm, ZRES wrote to [___+gva_slot_offset] which is garbage under the flat_lcl_proc frame layout -> SEGV (measured: global_test.icn CELLS=1 rc=139). SN4 watermark: is_global+!graph_has_local arm was already the sole SN4 path; _icn_cells=0 for SN4 -> byte-identical by construction.  ZK-2 FIX (s226): x86_fb_pinned() guard added to icn_cells_graph local arm -- IR_VAR/IR_ASSIGN locals use FRQ(varslot); on the RSP arm FRQ expands to [rsp+varslot+op_zdepth] which drifts as FORTH cells are carved.  In unpinned graphs (until/while condition sub-graphs, CALL-bearing graphs without flat_jmp_entry) ___ is the proc frame base but x86_fb_pinned()=0, so FRQ uses RSP-relative addressing and drifts: measured as n=parameter becoming equal to i=local after increment in rung09_loops_until (n was overwritten by i's RSP-relative write).  Pin guard ensures FRQ is ___-relative (depth-immune); pinned = flat_lcl_proc proc entry or jmp_entry blob with established ___.  The ZFRAME arm above already has this guard; the icn_cells arm now matches. */   /* ZD-2h-ICN (s211): global-non-local OR local-on-pinned (depth-immune-base).  SNOBOL4 invisible: graph_has_local returns 0 for all SN4 programs -- zero customers, zero behavior change, MEASURED s203.  The pin predicate (x86_fb_pinned) ensures the ___ frame is established before any [___-N] local access.  Named for WHAT: \"global, OR local on a depth-immune-base graph\".  ⛔ ZD-2h ORIGINAL COMMENT PRESERVED BELOW: RETIRED AS VACUOUS BY CONSTRUCTION -- DO NOT SPEND A RUNG WIDENING THIS LINE.  The carried NEXT item read "VAR-local/ASSIGN-local under the pin gate -- the depth-immune-base question, the C-style ___ flavor", i.e. arm locals only where ___ is pinned so a varslot read is not displaced by ZD depth.  MEASURED: the graph_has_local arm of this conjunct returns 0 for ZERO nodes across the whole crosscheck corpus, probed over every entry of nodes[] rather than inside the run loop (SCRIP_ZDLOCAL=1 below; the same probe flipped to the global arm fires 193 times in 40 programs, so the null is validated against a live control and is not an instrument artifact).  THE REASON IS SEMANTIC, NOT A CORPUS GAP: SNOBOL4 has no lexical locals.  A name in a DEFINE prototype's local list is an ORDINARY GLOBAL whose value is pushed on a stack at call, nulled, and restored on return (manual Ch.4 p.24 "any existing values ... will be saved on a pushdown stack ... restored to their previous values"; the formals get the identical treatment) -- which is law 7 of the s21x-c design of record, IR_SAVE_RESTORE handling them as globals in the one NV namespace.  graph_has_local is an ICON-era LEXICAL frame-slot concept (ARCH-ICON.md: Icon procedure params and local/static stay frame slots in both variable-model modes) and it has no SNOBOL4 customers at all.  There is no depth-immune-base question to answer here because there is no node class to answer it for.  The genuine ___ citizens of law 4 are the match family (ARBNO/FENCE1 variable-length housekeeping) and IR_CALL's frame dance -- that is where the "occasional ___" of the design lives, and it is where the remaining declines are. */
    if (op == IR_COERCE_NUMERIC || op == IR_CMP_TEST) return 1;   /* ZD-2d/2e: both grew ZD arms this rung and both dispatch to ONE template unconditionally (emit.cpp case IR_COERCE_NUMERIC / IR_CMP_TEST).  They are ARMED AS A PAIR deliberately: CMP_TEST's operands are the COERCE_NUMERIC outputs, so arming either alone leaves the statement declining on the other and measures nothing -- the 65 and the 38 live in the SAME statements. */
    if (op == IR_GOTO_DEFERRED) return 1;   /* ZD-8 (a) */
    if (op == IR_GOTO) return 1;   /* A-1/GE-A (s24b): IR_GOTO is a K=0 transparent relay -- zd_k already returns 0, zd_nops default 0 correct, bb_goto() emits ONLY x86_alpha()+x86_pair_loop() (no cell read, no cell write).  The planner's zd_chase() already threads through IR_GOTO transparently (while op==IR_GOTO: follow gamma), so the ONE semantic question is whether a length-1 IR_GOTO run should ARM rather than decline: it should, for the same reason IR_GOTO_DEFERRED and IR_STATEMENT do -- a transparent protocol box that carries no cell must not block the run it sits in.  MEASURED (roman.sno, s24b): 2 runs of len=1 IR_GOTO declined at r=0 while every sibling node in the graph admitted; admission unblocks those 2 runs and lets roman reach its IR_MATCH_BEGIN frontier (the remaining OMEGA-owned blocker).  GATEKEEP: controls 1016/1019/161/expr_eval must hold (the named eval-family falsification class); BY SET both modes vs open bracket. */
    if (op == IR_STATEMENT) return 1;   /* ⭐ ZD-4 (s23q) CROSS-FRONT REQUEST from OMEGA (O-1, `350b79ba`): IR_STATEMENT joins the K=0 transparent-sink class beside IR_ASSIGN and IR_GOTO_DEFERRED.  The box carries no cell (zd_k already returns 0), no operands (zd_nops default 0 is already correct), and no result to write -- it emits a label + the gamma-hook jmp whose X86H_JMP arm issues `add rsp, op_zgpop`; the STAGING of op_zgpop (in the dispatch case and in the run planner's terminal-pop computation) is the entire semantic content.  Without this line the minted box first-blocks its whole run, so ZW-5's lower-minted IR_STATEMENT boxes decline EVERY statement they wrap -- the gate says LIGHTING BLOCKED and the fix is here. */
    if (op == IR_FUNC_ACTIVATE) return 1;   /* AB: self-framing block, zero ζ cells yielded; always admitted transparent -- its own push/pop is its own responsibility, not a spine cell */
    if (op == IR_SAVE_RESTORE) { static int _sr = -1; if (_sr < 0) { const char * e = getenv("SCRIP_ZD_SR"); _sr = (e && *e == '0') ? 0 : 1; } return (_sr && zd_sr_role(nd) != 0) ? 1 : 0; }   /* ⭐⭐ ZD-SR: the SN4-FLAT-PROC linkage family admitted as a TRANSPARENT PROTOCOL BOX -- zero cells consumed (zd_nops default 0), zero cells yielded (zd_k 0), so it neither reads nor writes the value spine and must not disqualify the run that merely PASSES THROUGH it.  MEASURED (func_call.sno, the benchmark this rung was opened on): the proc body `INC = N + 1` lowers to var -> lit -> binop -> assign -> save_restore chained gamma-wise, so the trailing RETURN floater was breaking a run whose four value nodes were ALL already whitelisted; under SCRIP_NOFC=1 the producers carved their own cells at [rsp+0] while the declined consumer read the flat slot at [rsp+240] -- nobody's write -- and the benchmark printed `result: 0`.  THE ADMISSION IS NOT NEW REASONING: emit.cpp's STMT_FRAME classifier already calls this same kind "self-allocating by construction" in its _callfam arm, for the same three reasons -- role 0 carves and releases its OWN save block (bb_save_restore.cpp: "carved by its OWN single `sub rsp`, never a whole-graph carve, never ___-indexed"), roles 1/2 restore machine state WHOLESALE from the pcall record and jmp home, so abandoned depth behind them is by-design, and role 3 is the stub-entry wire-adopt that copies header wires before the body runs.  ROLE 0 STAYS DECLINED: it is the only role carrying the conditional g_emit-mutating CALL2BB arg-window preamble at the dispatch, and it exists only under SCRIP_CALL2BB=1 (lower_snobol4.c:190, default 0), so admitting it would arm an unmeasured path for zero benchmark gain -- the ZD-2l note that deferred this whole kind named exactly that preamble as the reason.  Killswitch SCRIP_ZD_SR=0. */
    if (op == IR_STATEMENT_BEGIN || op == IR_STATEMENT_END) return 1;   /* s26 (Lon directive): the statement bracket PAIR joins the K=0 transparent class its parent IR_STATEMENT earned at ZD-4 -- BEGIN is a label+jmp relay (frame arm dormant), END is the trailer re-kinded with identical staging; neither carries a cell, and an unadmitted first-blocker at every statement head would decline the whole corpus (the exact ZD-4 lesson, pre-paid here). */
    if (op == IR_KEYWORD_SNOBOL4) return 1;   /* ZD-2l: 13 first-blockers, dispatched to ONE template unconditionally (emit.cpp:880).  A ZERO-OPERAND leaf -- emit_chain_arity already classes it with the LIT_* producers at :2432 -- so zd_nops needs no arm and its default 0 is already right.  Admitted ALONE rather than with the rest of the tail: IR_SAVE_RESTORE carries a conditional g_emit-mutating preamble that is the CALL2BB arg-window linkage (:989), and IR_GOTO_DEFERRED is the EVAL/CODE runtime label transfer that flat_jmp_entry declines WHOLESALE by design -- both belong to the protocol rung, and cloning a value-spine arm onto either would be the 017 shape with extra steps. */
    if (op == IR_KEYWORD_ICON) { static int _icnkw = -1; if (_icnkw < 0) { const char * e = getenv("SCRIP_ZD_ICN_KW"); _icnkw = (e && *e == '0') ? 0 : 1; } if (!(_icnkw && g_emit_cfg && g_emit_cfg->icn_cells_graph)) return 0; { const char *_kw = IR_LIT(nd).sval; if (!_kw) return 0; const char *_k = (_kw[0] == '&') ? _kw + 1 : _kw; return (!strcmp(_k, "null") || !strcmp(_k, "pos") || !strcmp(_k, "subject")) ? 1 : 0; } }   /* ZK-2 ADDITIVE (s220): IR_KEYWORD_ICON admitted on the cells arm for SCALAR-SAFE subset only.  ADMITTED: &null ({DT_SNUL,0}), &pos ({DT_I,r14+1}), &subject ({DT_S,r13}) -- all return a simple 16B DESCR that consumers read via ZOPQ as a plain value; bb_keyword_icon.cpp ZD arm writes ZRES(0/8).  EXCLUDED: rt_keyword_read path (&output, &input, &errout, &maxlevel, etc.) -- MEASURED: &output via ZD arm SEGVd rung37_file_io because the file-handle DESCR was written to ZRES and consumer read the spine cell as the aggregate; non-ZD arm writes FRQ(op_off) correctly for these.  &fail excluded: pure-ω topology, cannot be first-blocker, no result cell.  Generator keywords excluded: IR_KEYWORD_ICON_GEN at lower_icon.c:319-320 (never reach this op).  zd_nops default 0 correct; K=16 default.  R13/R14 reads: read-only, not scratching per REGISTER CONTRACT.  SN4 watermark: icn_cells_graph=0 -> always 0 return -> byte-identical by construction.  Killswitch SCRIP_ZD_ICN_KW=0.  ONE AUTHORITY per s22k law -- extend the admitted-kw list here when consumer templates for new keywords are verified. */
    if (op == IR_COERCE_STRING || op == IR_COERCE_INTEGER) return 1;   /* ZD-2k: the coerce tail, 17 + 7 first-blockers in this rung's census.  Both dispatch to ONE template unconditionally (emit.cpp 892/893, the two lines ABOVE the already-armed IR_COERCE_NUMERIC at 894) so the 017 falsification shape cannot recur, and both templates grew by-address ZD arms this rung.  IR_COERCE_REAL is deliberately NOT here: it shares case 1354's lowering group but its own dispatch was not audited and it censused ZERO first-blockers, so admitting it would arm an unexercised template for no measurable gain -- the ZD-2f vacuity lesson applied in advance. */
    if (op == IR_DEREF || op == IR_ASSIGN_VAR) return 1;   /* ZD-2j: the two kinds ZD-2f's null exposed as the REAL subscript gate (51 + 51 of its 106).  Both dispatch to ONE template unconditionally (emit.cpp 993/995) and both are by-value clones of the proven arith shape with a genuine DT_FAIL omega.  Armed TOGETHER with each other and WITH the already-written 2f subscript arm, which they unblock. */
    if (op == IR_FIELD_VAR) return 1;   /* ZD-2m: the LAST value-spine first-blocker, 5 of them, and with it the census is ALL PROTOCOL (CALL 518 / MATCH_BEGIN 247 / SAVE_RESTORE 18 / GOTO_DEFERRED 6).  IR_FIELD_GET is DELIBERATELY NOT admitted beside it even though the two share one dispatch line (:1052) and one template: it is built ONLY by lower_icon.c:418 and lower_raku.c:445 and has ZERO SNOBOL4 customers, so admitting it would arm an unexercised kind for two frontends whose ladders have not validated ZD -- the ZD-2b ruling ("reachable only from another frontend, so it belongs to that language's ladder") applied to a kind that happens to be a co-tenant of this one's template rather than of its own.  The ZD arm's lv ternary still spells the FIELD_GET call target, so that ladder inherits a written arm when it arms the kind, exactly as ZD-2j inherited 2f's. */
    if (op == IR_SUBSCRIPT) return (nd->n_operands == 2) ? 1 : 0;   /* ZD-2f: the RVALUE x[i] form ONLY.  emit.cpp:992 routes 2-operand IR_SUBSCRIPT to bb_subscript (which grew a ZD arm) and every other arity to bb_section (which did NOT) -- arming by bare kind would hand a staged op_zres to a template that ignores it, the exact 017 falsification shape. */
    if (op == IR_CALL) { extern int rt_proc_is_registered(const char *); extern int rt_proc_is_generator(const char *); const char * fn = IR_LIT(nd).sval; static int _zp = -1; if (_zp < 0) { const char *e = getenv("SCRIP_ZD_PROC"); _zp = (e && *e == '0') ? 0 : 1; } if (fn && rt_proc_is_registered(fn)) { if (!_zp) return 0; return (rt_proc_is_generator(fn)) ? 0 : 1; } return 1; }   /* ZD-7 (a): bare IR_CALL admitted EXCEPT user-defined procs.  ZD-7c (s23r): under SCRIP_ZD_PROC (default ON after bb_call_proc_staged gains its ZD arm) registered non-generator procs ARE admitted -- the template's ZD early-return uses ZOPQ(k,0/8) for arg reads and ZRES(0/8) for result writes, avoiding the FRQ(resoff)+op_zdepth wrong-address defect that 085/086/087 blank-output measured.  Generators stay DECLINED: suspended activation state (retained RSP frontier, pcall record) is incompatible with the ZD cell model -- the callee may resume at arbitrary RSP depth, so op_zread differences computed at emit time are not valid at resume time.  KILLSWITCH SCRIP_ZD_PROC=0 restores the pre-7c exclusion byte-for-byte.  The op==IR_CALL_VALUE exclusion is implicit: IR_CALL_VALUE is a different op code, not reached here. */
    if (op == IR_CALL_BUILTIN_ICON) { extern int icn_builtin_is_generator(const char *); extern int icn_builtin_is_known(const char *); const char * fn = IR_LIT(nd).sval; if (fn && icn_builtin_is_generator(fn)) return 0; /* ZK-3: every-body-decline REMOVED -- op_sb=1 staged at dispatch handles this topology in the ZD arm instead. */ if (fn && icn_builtin_is_known(fn)) return 1; return 1; }   /* ZD-ICN-CALL (s208): IR_CALL_BUILTIN_ICON admitted for non-generator builtins.  ROOT CAUSE THIS CLOSES: CARVE-KILL deleted the whole-graph flat frame; the legacy arm of bb_call_fn_str reads FRQ(argbase) offsets into that deleted frame, writing [rsp+16]/[rsp+24] above the FORTH frontier and scribbling caller-saved rdi/rsi -- the exact SEGV in write("hello").  The ZD arm of bb_call_fn_str already exists (ZD-7c shape: sub rsp,nargs*16 / copy ZOPQ predecessors / lea rsi,[rsp+0] / call rt_call_arr / add rsp / ZRES write) -- this admission line is the ONLY missing piece.  Generators excluded: suspended RSP frontier is incompatible with op_zread depth-difference arithmetic, same reason as ZD-7 generators.  bb_call_route_classify routes CALL_BUILTIN_ICON to CALL_ROUTE_FN (known non-gen) or CALL_ROUTE_BYNAME (generator), both of which already carry ZD arms; unknown builtins fall through to CALL_ROUTE_BYNAME via the final else -- also ZD-armed -- so return 1 is safe for the unknown arm too.  SNOBOL4 watermark: SNOBOL4 does not use IR_CALL_BUILTIN_ICON so this line is INVISIBLE to the SN4 crosscheck set by construction. */
    if (op == IR_MATCH_BEGIN || op == IR_MATCH_END || op == IR_MATCH_REPLACE) return 1;   /* ⭐ ZD-5 MATCH-SPINE admission UNCONDITIONAL (Lon 2026-08-07 REGIME DELETION: "You DELETE the other regime").  SCRIP_ZD_MATCH env gate DELETED -- the declined/legacy regime is being physically removed; wholesale is the only path.  Prior default-OFF rationale (SEQ-ERAD s9 crater 81/34/12/14) and re-measure history: git log this line + FINDING-2026-08-07e.  Crater at this flip recorded in the commit message per GATE POLICY. */
    { static int _za = -1; if (_za < 0) { const char * e = getenv("SCRIP_ZD_ALT"); _za = (e && *e == '0') ? 0 : 1; }
      if (_za && op == IR_MATCH_ALTERNATE) return 1; }   /* ⭐ ZD-5b ALTERNATE (O-7/ALPHA A-8 cross-front, s29): K=0 envelope -- zero own cell, scanner-register-only; bb_match_alternate ALT-FLAT arm uses address dispatch (FRQ(op_off+8/16) = resume/next pointers, FR(op_off) = saved r14d) which is per-BB frame-relative and ZD-compatible.  zd_plan subwalk descends arm entries via operands[2i]; nops default 0 (no inline run-predecessor: arm entries are subwalked by the planner, not staged as depth-difference reads).  Killswitch SCRIP_ZD_ALT=0.  s22k one-authority: zd_k below and this site only. */
    { static int _zdf2 = -1; if (_zdf2 < 0) { const char *_e = getenv("SCRIP_ZD_FENCE1"); _zdf2 = (_e && *_e == '0') ? 0 : 1; }
      if (_zdf2 && op == IR_MATCH_FENCE1) return 1; }   /* ⭐ O-7 ZD-5b FENCE1 (s30, default ON): FENCE1 is a law-4 ___ user whose watermark slot FRQ(off)=[___+off] is depth-free under the ZW canonical frame (___ = match frame base, depth-immune). bb_match_fence1's fence_whack_commit uses mov rsp,[___+0] (activation floor = pushed old-___) under op_zw, not mov rsp,___ (match frame base), so the bulk-whack targets the correct floor. K=0 (zd_k): FENCE1 yields no result cell; its own storage is the granted watermark quad, not a 16B spine cell. nops=0 default correct (no inline run predecessors; the N-ary operand children are subwalked, not staged as depth-difference reads). Killswitch SCRIP_ZD_FENCE1=0 restores the excluded regime byte-for-byte (backed by the zdyn veto + seal gate restorations above). s22k one-authority: zd_k below and this site only. */
    /* ⭐ ZD-5b BLOB-INTERIOR ADMISSION (A-7, s24b): K=0 leaf scanner kinds admitted KIND BY KIND after (a) template ZD arm, (b) wpop beta-tag fix, and (c) Kc has_blob gate fix — all three landed s24b.  ARBNO stays out (INDETERMINABLE, OMEGA's ___ terrain).  ADMITTED: LIT (s24b). */
    if (op == IR_MATCH_DEFER && nd->pat_static) return 1;   /* ⭐ ZD-PATREF (OMEGA s39): the stored-pattern INVOKE box admitted as a transparent transfer citizen.  K=0 in zd_k (transfer box, no spine cell): bb_match_defer enters the blob via bb_glue_pass_wires + jmp rax; the match yields through scanner registers/capture side-effects, never a 16B ZRES cell; op_off reads are zls-GRANTED slot reads (seal==1 arm only), not ZRES consumers.  There IS no op_zres arm to land -- DEFER is not a value producer.  The "017 falsification" concern (verdict staged for a template that ignores it) does not apply: zd_wl_kind=1 + K=0 only means "this node contributes 0 bytes to the claim", which is already correct.  TWO ADMISSION PATHS: (a) SCRIP_ZD_PATREF=1 (whole-kind override, opt-in measurement); (b) nd->pat_static=1 (per-node: PATV$ hidden globals, write-once-frozen by PB-1s construction, W-2a stamp).  Path (b) is the production path for the PB-1s snapshot DEFER class; path (a) keeps the global measurement knob.  Killswitch: SCRIP_ZD_DYNARM=0 still forces st=0 in the zdyn veto, so the admitted node's run still declines if that override is in effect. */
    if (!(g_emit_cfg && (g_emit_cfg->icn_cells_graph || g_emit_cfg->pl_cells_graph))) return 1;   /* WL-DELETE (Lon 2026-08-08): kind whitelist RETIRED for non-cells graphs -- every BB admitted; Icon/Prolog cells registries below retained verbatim (do-not-break); zframe early-return above unchanged. */
    if (op == IR_MATCH_LIT) return 1;   /* ZD-5b (s24b): bb_match_lit arm + wpop beta-tag fix + Kc has_blob gate fix.  K=0 (zd_k), nops=0 default correct, scanner-register-only body. */
    if (op == IR_MATCH_LEN)   return 1;   /* ZD-5b (s25): bb_match_len arm.  K=0, nops=nd->n_operands, dynamic arm reads ZOPQ(0,8). */
    if (op == IR_MATCH_ANY)   return 1;   /* ZD-5b (s25): bb_match_any arm.  K=0, nops=nd->n_operands, static body identical; dynamic arm reads ZOPQ(0,8)/ZOPD(0,4) for charset ptr/len. */
    if (op == IR_MATCH_NOTANY) return 1;  /* ZD-5b (s25): bb_match_notany arm.  K=0, nops=nd->n_operands, same shape as ANY. */
    if (op == IR_MATCH_POS)   return 1;   /* ZD-5b (s25): bb_match_pos arm.  K=0, zero-width predicate, nops=nd->n_operands, dynamic arm reads ZOPQ(0,8). */
    if (op == IR_MATCH_RPOS)  return 1;   /* ZD-5b (s25): bb_match_rpos arm.  K=0, zero-width predicate, nops=nd->n_operands, dynamic arm reads ZOPQ(0,8). */
    if (op == IR_MATCH_TAB)   return 1;   /* ZD-5b (s25): bb_match_tab arm.  K=16 (needs cursor save across beta in ZRESD(0)); nops=nd->n_operands; dynamic arg ZOPQ(0,8). */
    if (op == IR_MATCH_RTAB)  return 1;   /* ZD-5b (s25): bb_match_rtab arm.  K=16; same scratch shape as TAB; target from end (r15d-arg). */
    if (op == IR_MATCH_REM)   return 1;   /* ZD-5b (s25): bb_match_rem arm.  K=16; cursor save in ZRESD(0); no arg (always static). */
    { static int _zb = -1; if (_zb < 0) { const char * _e = getenv("SCRIP_ZD_BREAK"); _zb = (_e && *_e == '0') ? 0 : 1; } if (_zb && (op == IR_MATCH_BREAK || op == IR_MATCH_BREAKX)) return 1; }   /* ZD-5b (s26): bb_match_break/breakx arms.  K=16 (default -- scratch slot _.x86_scratch_off saves old r14d / scan delta across beta, same shape as TAB/RTAB/SPAN).  nops=nd->n_operands: static form has _.op_sa<0 and zero operands; dynamic form _.op_sa>=0 has operand[0]=charset DESCR, read ZOPQ(0,8)/ZOPD(0,4) by the template (bk_ndl_r8/bk_ndl_rsi dispatch: r8=ptr/r9d=len or rsi=ptr/edx=len).  No op_zres result cell (scanner-register cursor advance only, like SPAN/TAB/RTAB).  Killswitch SCRIP_ZD_BREAK=0. */
    if (op == IR_MATCH_SPAN)  return 1;   /* ZD-5b (s25): bb_match_span arm.  K=16; ZRESD(0)=old r14d, ZRESD(4)=scan counter; dynamic charset ZOPQ(0,8)/ZOPD(0,4). */
    { static int _zc = -1; if (_zc < 0) { const char * _e = getenv("SCRIP_ZD_CAP"); _zc = (_e && *_e == '0') ? 0 : 1; }   /* ZD-5b CAPTURE (s32, default ON -- flip: staging loop special-case COND/IMM operands[1] landed s27 emit.cpp:2533; bb_match_capture ZD arms landed s27 OMEGA; BY SET identical both modes 290/28 pre/post -- see [ALPHA] s32 FINDING.  SCRIP_ZD_CAP=0 reverts byte-for-byte.  VALUE stays excluded: s31 FINDING shows VALUE admission alone promotes proc_PAT$0 runs containing IR_MATCH_CALL (jmp-entry structure) -- those calls segfault under ZD until OMEGA arms their templates.  SAVE stores r14d at ZRES(0) (K=16 own cell); COND/IMM read SAVE's cell via ZOPQ(1,0) = staged op_zread[0] from operands[1]. */
      if (_zc && (op == IR_MATCH_ASSIGN_SAVE || op == IR_MATCH_ASSIGN_COND || op == IR_MATCH_ASSIGN_IMM)) return 1;   /* ZD-5b (s27): bb_match_capture ZD arm needed in OMEGA-owned file.  SAVE: op_zres gate + store r14d to ZRES(0) (replaces sfc() rspd(0) store, which fires cap_sym()+op_fc_base<0=false under ZD, falling to rt_cap_push -- wrong path).  COND/IMM: op_zres gate + read ZOPQ(1,8) (SAVE's cursor delta via depth-diff) replacing cfc() rspd(op_fc_disp) cross-depth read; then r14d - ZOPQ(1,8) = match length, rest of pend-push/CAS store unchanged. */
      if (_zc && op == IR_MATCH_VALUE) return 1; }   /* ZD-5b (s27): bb_match_value ZD arm needed (OMEGA-owned).  operand[0] = value-spine pattern DESCR; ZD arm reads ZOPQ(0,8) replacing FR(op_a_slot).  K=0 (no own result cell; cursor advance via r14d only).  s31 FINDING: VALUE admission alone (split from SCRIP_ZD_CAP) promotes proc_PAT$0 VALUE-bearing runs from UCLAIM to ZD-armed, but those runs contain IR_MATCH_CALL nodes (pattern-function invocations with jmp-entry structure) that segfault under ZD -- the UCLAIM path was correct for them.  VALUE must be re-admitted only after those run members' templates have ZD arms, which is OMEGA work. */
    /* ZD-5 NOTE (s22i): IR_MATCH_BEGIN is NOT admitted here yet.  The 247 "MATCH_BEGIN declines" in the census
     * mean MATCH_BEGIN was the FIRST blocker in the run -- but the ACTUAL blocker keeping those runs declined
     * is the subsequent pattern blob nodes (IR_MATCH_LIT, IR_MATCH_ALT, IR_MATCH_ASSIGN_SAVE, etc.) that have
     * no ZD arms.  Admitting MATCH_BEGIN alone causes a segfault: the run partially arms (HEAD passes), then
     * falls into the pattern blob at a depth offset the flat FRQ reads don't expect.  Pattern blob ZD arming
     * is a future rung; MATCH_BEGIN admission belongs on that rung, not here. */
    /* ⛔ SCRIP_ZD_TOTAL IS A MEASUREMENT PROBE, NOT A FEATURE FLAG -- AND THE DISTINCTION IS THE WHOLE POINT OF THIS FUNCTION.  Flipping it default-admits
     * every kind outside the pattern-blob block and scores Icon 238/25/30 -> 130/133/30.  It does NOT "turn allocation on for every BB"; it makes this
     * function LIE.  zd_wl_kind is not a policy whitelist that someone was being timid with -- it is a CAPABILITY REGISTRY recording which kinds have a ZD
     * arm IMPLEMENTED IN THEIR TEMPLATE.  Admitting a kind whose template has no ZD arm arms the node without anyone writing its cell, so its consumer
     * reads an unwritten rsp slot.  The ledger entry "IR_CALL_BUILTIN_ICON -- 68 declines, absent from the whitelist; likely ONE TEMPLATE ZD ARM" says
     * exactly this: the unit of work is a template arm, added one kind at a time, after which the kind is admitted here and the registry becomes total BY
     * CONSTRUCTION.  The probe's value is that its delta MEASURES the size of the remaining capability gap; use it to rank which arm to write next, never
     * to ship.  Keep it default-off. */
    if (op == IR_TO) return 1;   /* ZD-ICN-TO (s211): IR_TO admitted -- bb_to.cpp ZD arm (K=32: result 16B + counter ZLOC(0) 16B) reads lo/hi from ZOPQ(0/1), counter at ZLOC(0)=[rsp+16], result at ZRES(0/8); β increments counter and re-enters loop without release (NON-POPPING FORTH spine -- cells persist to statement boundary where op_zgpop restores rsp).  zd_nops already returns 2 for IR_TO (line 2756, THE ONE AUTHORITY).  zd_k returns 32 for IR_TO (THE ONE AUTHORITY below).  SNOBOL4 watermark: SNOBOL4 does not emit IR_TO -- invisible to the SN4 crosscheck set by construction. */
    /* ⭐⭐ PL-ZK-1: PROLOG ADMISSION ARMS (s3) -- five additive arms, each gated on pl_cells_graph + own killswitch.  ONE AUTHORITY per s22k law -- only zd_wl_kind and zd_k name each kind for the ZD regime.  SN4/Icon watermarks: pl_cells_graph is set only by lower_prolog.c; every other lowerer produces pl_cells_graph=0 by calloc -- byte-identical by construction.  Each arm: K=16 default (see zd_k below); template ZD arm lands at the rung following this one. */
    if (op == IR_CALL_BUILTIN_PROLOG) { static int _plcbp = -1; if (_plcbp < 0) { const char * e = getenv("SCRIP_ZD_PL_CBP"); _plcbp = (e && *e == '0') ? 0 : 1; } return (_plcbp && g_emit_cfg && g_emit_cfg->pl_cells_graph) ? 1 : 0; }   /* PL-ZK-1 arm 1/5: IR_CALL_BUILTIN_PROLOG admitted on the cells arm.  Census (s3): first-blocker in every queens.pl graph (4/7/19 occurrences); gates 185/185 runs per WHAT-THE-PRIOR-SCANS-FOUND item 1 (the unlock-set finding).  K=16 default; nops = nd->n_operands (already covered by IR_CALL_BUILTIN_PROLOG's membership in the n_operands arm of zd_nops; verify: grep IR_CALL_BUILTIN_PROLOG zd_nops ONE AUTHORITY).  Killswitch SCRIP_ZD_PL_CBP=0. */
    if (op == IR_VAR_REF) { static int _icnvr = -1; if (_icnvr < 0) { const char * e = getenv("SCRIP_ZD_ICN_VR"); _icnvr = (e && *e == '0') ? 0 : 1; } if (_icnvr && g_emit_cfg && g_emit_cfg->icn_cells_graph && x86_fb_pinned()) { const char * _vn = IR_LIT(nd).sval; extern int is_global(const char *); if (_vn && !is_global(_vn)) return 1; }   /* ⭐ ZK-2 ADDITIVE: IR_VAR_REF on the ICN cells arm, pinned+local only.  Template arm in bb_var_ref.cpp (op_zres && op_sa >= 0 && !pl_cells_graph) writes NAMETRAP {DT_N,slen=1,&____slot} to ZRES; consumed by IR_DEREF ZD arm (already admitted at :1920) via rt_deref.  x86_fb_pinned(): depth-immune ___ established by flat_lcl_proc prologue -- FRQ(op_sa)=[___+op_sa] is stable across subsequent RSP carves (the s221 LEA-DRIFT law: without pin the baked [rsp+op_sa+depth] drifts per carve).  !is_global(): global VAR_REFs route to GVA (process-lifetime ζ, ZK-5 law; armed global would write NAMETRAP to GVA memory, wrong layout).  K=16 default (zd_k).  nops=0 default (zd_nops): VAR_REF reads a named frame slot, not a spine-predecessor cell -- no ZOPQ read staged.  SN4/Prolog watermark: icn_cells_graph=0 for all non-Icon graphs -> always skips -> byte-identical by construction.  Killswitch SCRIP_ZD_ICN_VR=0.  ONE AUTHORITY per s22k law. */ static int _plvr = -1; if (_plvr < 0) { const char * e = getenv("SCRIP_ZD_PL_VR"); _plvr = (e && *e == '0') ? 0 : 1; } return (_plvr && g_emit_cfg && g_emit_cfg->pl_cells_graph && !g_emit_cfg->pl_cells_graph) ? 1 : 0; }   /* PL-ZK-5B GATE: VAR_REF Prolog cells admission disabled -- needs TERM_VAR materialization (s13). Tautology !pl_cells_graph&&pl_cells_graph=always-0 preserves killswitch structure. */   /* PL-ZK-1 arm 2/5: IR_VAR_REF admitted on the cells arm.  Census (s3): second-most-frequent blocker (13/6/35/9 occurrences).  K=16 default; nops=0 (no inline run-predecessor -- VAR_REF reads a named variable slot, not a spine cell; template ZD arm will read from slot).  Killswitch SCRIP_ZD_PL_VR=0.  ⛔ ZK-2 ICON CELLS ARM DEFERRED (s220/s221): Icon VAR_REF cells-arm admission was attempted twice and reverted both times.  TWO INDEPENDENT BLOCKERS measured s221 (see FINDING-2026-08-08-CLAUDE-SN46-ZK2-VAR-REF-TWO-BLOCKERS...md): (1) UNPINNED GRAPH LEA DRIFT -- `lea rdx,FRQ(op_sa)` on rsp-arm bakes [rsp+op_sa+depth]; subsequent sub rsp,K carves move rsp so the stored address drifts; rung23_table_basic silent-wrong (rsp+288 at n3 and n8 reach different absolute memory after 80B of carves).  Pin guard x86_fb_pinned() fixes this but exposes (2) LVALUE/RVALUE CONSUMER SPLIT -- rt_subscript_var ALWAYS returns NAMETRAP {DT_N,slen=2,vc}; lvalue consumers (rt_assign_var) handle it correctly; rvalue consumers (right()/write() via IR_CALL_BUILTIN_ICON) expect a plain value DESCR; IR_DEREF (the rvalue-extraction node, already has a full ZD arm in bb_deref.cpp) is NOT yet admitted on the cells arm, so VAR_REF+SUBSCRIPT+DEREF cannot arm atomically and the partial-arm geometry truncates output (jcon_kross xprint flat_gen 13/28 armed).  s220 DIAGNOSIS CORRECTED: rt_subscript_var/rt_assign_var/rtx_icnsub.S all already handle {DT_N,slen=1} correctly via IS_VARREF_fn+rt_deref -- no consumer template changes needed.  NEXT ATTEMPT: arm IR_DEREF (zd_wl_kind one line, bb_deref.cpp ZD arm already exists), then re-admit VAR_REF with x86_fb_pinned()+!is_global() guards -- the run gate then ensures VAR_REF+SUBSCRIPT+DEREF arm atomically, preventing the partial-arm NAMETRAP hazard. */
    if (op == IR_CALL_PROC_STAGED) { static int _plcps = -1; if (_plcps < 0) { const char * e = getenv("SCRIP_ZD_PL_CPS"); _plcps = (e && *e == '0') ? 0 : 1; } return (_plcps && g_emit_cfg && g_emit_cfg->pl_cells_graph) ? 1 : 0; }   /* PL-ZK-1 arm 3/5: IR_CALL_PROC_STAGED admitted on the cells arm.  Census (s3): third-blocker class.  K=16 default; nops = nd->n_operands.  Killswitch SCRIP_ZD_PL_CPS=0. */
    if (op == IR_SUSPEND) { static int _plsusp = -1; if (_plsusp < 0) { const char * e = getenv("SCRIP_ZD_PL_SUSP"); _plsusp = (e && *e == '0') ? 0 : 1; } return (_plsusp && g_emit_cfg && g_emit_cfg->pl_cells_graph) ? 1 : 0; }   /* PL-ZK-1 arm 4/5: IR_SUSPEND admitted on the cells arm.  Census (s3): appears in every queens.pl graph (1/1/2/2 occurrences).  K=16 default; nops=0 (SUSPEND carries two operand refs to the generator graph -- not inline run-predecessors in the stack sense; the ZD arm will address them via slots).  Killswitch SCRIP_ZD_PL_SUSP=0. */
    if (op == IR_VAR) { static int _plvar = -1; if (_plvar < 0) { const char * e = getenv("SCRIP_ZD_PL_VAR"); _plvar = (e && *e == '0') ? 0 : 1; } return (_plvar && g_emit_cfg && g_emit_cfg->pl_cells_graph) ? 1 : 0; }   /* PL-ZK-1 arm 5/5: IR_VAR admitted on the cells arm.  Census (s3): appears as blocker (slen=0 NAMEVAL class, Prolog logic variables).  ⚠ GOAL-PL-ZETA-CELLS item 4 WARNING: the \"RETIRED AS VACUOUS\" ruling was for SN4 GLOBALS; Prolog vars ARE graph locals (single-assignment, trail-backed).  This arm is gated on pl_cells_graph so it is structurally Prolog-only -- the SN4 IR_VAR path is byte-identical by construction (pl_cells_graph=0).  K=16 default; nops=0.  Killswitch SCRIP_ZD_PL_VAR=0. */
    if (op == IR_DISJUNCTION) { static int _pldj = -1; if (_pldj < 0) { const char * e = getenv("SCRIP_ZD_PL_DJ"); _pldj = (e && *e == '0') ? 0 : 1; } return (_pldj && g_emit_cfg && g_emit_cfg->pl_cells_graph && nd->n_operands == 0) ? 1 : 0; }   /* PL-ZK-4 additive: IR_DISJUNCTION (0-operand Prolog form) admitted on the cells arm.  K=0: pure control-dispatch box -- α is `rt_pl_retry_pop -> jmp rax` (ZD arm) vs `jmp *FRQ(op_off+16)` (legacy); carries no own result cell on the FORTH spine.  The FRQ(op_off+16) continuation pointer written by IR_MOVE_LABEL migrates to g_pl_retry (rt_pl_retry_push/pop, PL-FR-4 `a0196747`) under ZD.  n_operands==0 guard: Icon nary form (n_operands>0) stays on bb_disjunction() -- byte-identical for Icon by construction.  SN4/Icon watermarks: pl_cells_graph=0 for all non-Prolog graphs -- byte-identical by construction.  Killswitch SCRIP_ZD_PL_DJ=0.  ONE AUTHORITY per s22k law -- only zd_wl_kind and zd_k name this kind for the ZD regime. */
    if (op == IR_CUT) { static int _plcut = -1; if (_plcut < 0) { const char * e = getenv("SCRIP_ZD_PL_CUT"); _plcut = (e && *e == '0') ? 0 : 1; } return (_plcut && g_emit_cfg && g_emit_cfg->pl_cells_graph) ? 1 : 0; }   /* PL-ZK-1 additive: IR_CUT admitted on the cells arm.  Census (s3): appears as blocker in queens.pl choice-point graphs.  K=0 (transparent -- CUT clears choice points above the cut barrier; no own result cell; the trail/CP side effect is pure runtime).  zd_nops default 0 correct (no inline run-predecessor).  Killswitch SCRIP_ZD_PL_CUT=0.  ONE AUTHORITY per s22k law. */
    if (op == IR_MOVE_LABEL) { static int _plml = -1; if (_plml < 0) { const char * e = getenv("SCRIP_ZD_PL_ML"); _plml = (e && *e == '0') ? 0 : 1; } return (_plml && g_emit_cfg && g_emit_cfg->pl_cells_graph) ? 1 : 0; }   /* PL-ZK-1 additive: IR_MOVE_LABEL admitted on the cells arm.  Census (s3): appears as blocker in multi-clause predicate graphs.  K=0 (wire -- MOVE_LABEL updates the dynamic jump target; no own result cell).  zd_nops default 0 correct.  Killswitch SCRIP_ZD_PL_ML=0.  ONE AUTHORITY per s22k law. */
    if (op == IR_MAKE_LIST) { static int _icnml = -1; if (_icnml < 0) { const char * e = getenv("SCRIP_ZD_ICN_ML"); _icnml = (e && *e == '0') ? 0 : 1; } return (_icnml && g_emit_cfg && g_emit_cfg->icn_cells_graph && x86_fb_pinned()) ? 1 : 0; }   /* ⭐ ZK-2 ADDITIVE: IR_MAKE_LIST on the ICN cells arm.  K=16 (result DESCR 16B at ZRES(0/8); zd_k default).  nops=nd->n_operands (all element predecessors are inline run-predecessors, each staged as ZOPQ(k,0/8) — just added to the n_operands arm of zd_nops above; ONE AUTHORITY).  ZD arm in bb_make_list.cpp: sub rsp,n*16 scratch; for each k: copy ZOPQ(k,0/8) to [rsp+k*16]; lea rdi,[rsp+0]; call rt_make_list; add rsp,n*16; write result to ZRES(0/8); x86_gamma().  No DT_FAIL omega: rt_make_list always succeeds (no failure mode).  x86_fb_pinned() guard: op_off=drive_value_slot(nd)=FRQ(resultslot)=[___+off]; on unpinned RSP graphs FRQ drifts per carve (the s221 LEA-DRIFT law).  gated icn_cells_graph: SN4/Prolog byte-identical by construction (lower_snobol4.c/lower_prolog.c never emit IR_MAKE_LIST).  Killswitch SCRIP_ZD_ICN_ML=0.  ONE AUTHORITY per s22k law. */
    if (op == IR_RETURN) { static int _zk4r = -1; if (_zk4r < 0) { const char * e = getenv("SCRIP_ZD_RETURN"); _zk4r = (e && *e == '0') ? 0 : 1; } return (_zk4r && g_emit_cfg && g_emit_cfg->icn_cells_graph) ? 1 : 0; }   /* ⭐ ZK-4 ADDITIVE: IR_RETURN admitted on the cells arm -- K=0 (terminal transfer, no own result cell; zd_k K=0 class); nops=1 (the return expression, read via ZOPQ(0,0/8)); ZD arm in bb_return.cpp reads ZOPQ then writes FRQ(0)/FRQ(8) (result slot, ___-relative, depth-immune under flat_lcl_proc's pinned ___). op_zgpop at gamma releases the full K_total spine depth. GATED icn_cells_graph: SN4/Prolog/Raku byte-identical by construction. Generator return (op_dval==2.0, ω exit) not yet on cells arm -- ZK arm gates on !flat_gen || op_dval!=2.0 inside bb_return.cpp. Killswitch SCRIP_ZD_RETURN=0. ONE AUTHORITY per s22k law. */
    if (op == IR_CONJUNCTION) { static int _zk2c = -1; if (_zk2c < 0) { const char * e = getenv("SCRIP_ZD_CONJ"); _zk2c = (e && *e == '0') ? 0 : 1; } return (_zk2c && g_emit_cfg && g_emit_cfg->icn_cells_graph) ? 1 : 0; }   /* ⭐ ZK-2 ADDITIVE: IR_CONJUNCTION admitted on the cells arm -- K=0 transparent alias: the conjunction aliases its operand's slot (bb_slot_register at dispatch) so op_sa==op_off in the common case; the template emits x86_alpha()+x86_pair_loop() (no cell read, no cell write). zd_k returns 0 (one-authority below); zd_nops returns 0 (default correct -- the alias is not a staged predecessor read). SNOBOL4 watermark: SN4 does not emit IR_CONJUNCTION; byte-identical by construction. ZFRAME watermark: cells arm is exclusive with zframe_graph. Killswitch SCRIP_ZD_CONJ=0. ONE AUTHORITY: only this line and zd_k name this kind for the ZD regime. */
    if (op == IR_BINOP_TEST) { static int _zk2r = -1; if (_zk2r < 0) { const char * e = getenv("SCRIP_ZD_RELOP"); _zk2r = (e && *e == '0') ? 0 : 1; } return (_zk2r && g_emit_cfg && g_emit_cfg->icn_cells_graph) ? 1 : 0; }   /* ⭐ ZK-2 ADDITIVE: IR_BINOP_TEST admitted on the cells arm -- K=16 {0,1} generator (ARCH-ICON: relational ops γ yield a value, ω fail; ground truth refs/jcon-master/tran/irgen.icn ir_opfn with failLabel set for relops). zd_nops=2 (lhs=operands[0], rhs=operands[1]); template ZD arm reads ZOPQ(0,0/8)=lhs ZOPQ(1,0/8)=rhs and writes ZRES(0/8)=result. SNOBOL4 watermark: SN4 does not emit IR_BINOP_TEST; byte-identical by construction. Killswitch SCRIP_ZD_RELOP=0. ONE AUTHORITY: only this line and zd_nops name this kind for the ZD regime. */
    if (op == IR_BOUND || op == IR_UNMARK) { static int _zk3 = -1; if (_zk3 < 0) { const char * e = getenv("SCRIP_ICN_CELLS"); _zk3 = (e && *e == '1') ? 1 : 0; } return (_zk3 && g_emit_cfg && g_emit_cfg->icn_cells_graph) ? 1 : 0; }   /* ⭐ ZK-3 (s213): IR_BOUND/IR_UNMARK admitted on the cells arm only -- K=0 transparent bracket pair (no own cell; zd_k lists them in the K=0 class).  BOUND α saves RSP at bounded-expression entry; UNMARK α restores RSP via op_zgpop (= accumulated K_total since BOUND entry, computed by zd_plan's exit-edge test: the UNMARK's gamma exits the bound, so zgpop[unmark] = depth_at_unmark_entry).  Semantics: interp.r Op_Mark (save efp)/Op_Unmark (rsp=efp-1) -- the per-lap rsp restore that reclaims abandoned suspension carves inside every-do bodies.  SNOBOL4 watermark: SN4 never emits IR_BOUND/IR_UNMARK (lower_snobol4.c has no analogue); cells flag never set for SN4; byte-identical by construction.  ZFRAME watermark: zframe_graph and icn_cells_graph are mutually exclusive (routing law R-ZK-A); ZFRAME's bb_bound slot-restore path is the fallback when cells=0.  Killswitch: SCRIP_ICN_CELLS=0 returns 0 here -> falls through to today's slot-based path.  ONE AUTHORITY per s22k law -- only zd_wl_kind and zd_k name these kinds; dispatch case below already stages op_sb/op_off; the cells arm in bb_bound.cpp reads op_zgpop. */
    if (op == IR_LIT_INTEGER || op == IR_LIT_STRING || op == IR_LIT_REAL || op == IR_LIT_CHARSET) return 1;   /* ZD-ICN-LIT (s211): lit kinds admitted -- bb_lit_scalar already has a ZD arm (ls_rq/ls_rd switch on op_zres; writes ZRES(0/8) when armed).  K=16 (default).  No operands (zd_nops default 0).  These are the predominant producers on the Icon value spine; every run containing them was declining at r=0 before this line, preventing any downstream consumer (IR_TO, IR_CALL_BUILTIN_ICON) from receiving staged op_zread depths.  SNOBOL4 watermark: SN4 lit kinds are gated through fc_geom (ZB-VAL-0 fixed-cell grant) and keep op_zres=0 when fc_geom fires; this line adds ZD-capability but the fc_geom conjunct in the dispatch (line 883) retains byte-identity for any SN4 lit that gets a fixed-cell grant.  THE ONE AUTHORITY per the s22k law. */
    { static int _tot = -1; if (_tot < 0) { const char * e = getenv("SCRIP_ZD_TOTAL"); _tot = (e && *e == '1') ? 1 : 0; }
      if (_tot) return (op >= IR_MATCH_LIT && op <= IR_MATCH_ADVANCE) ? 0 : 1; }
    return 0;
}
/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int zd_nops(IR_t * nd) { int op = (int)nd->op;
    if (op == IR_CALL_PROC_STAGED && g_emit_cfg && g_emit_cfg->pl_cells_graph) return 0;   /* PL-ZK-4b ONE AUTHORITY: IR_CALL_PROC_STAGED on pl_cells_graph has nops=0 for the ZD planner.  Prolog proc-call args are delivered via rt_arg_stage/g_call_args[] (the inline-stage path in bcps_det_arm -- not ZOPQ spine reads); the planner's operand-in-run check would fail for every argument subgraph that is not on the linear gamma-chain run.  Setting nops=0 here tells zd_plan there are zero in-run operand predecessors to validate, which is correct: the ZD arm reads ZRES(0/8) for its own result cell and the arg DESCR sources come from FRQ(arg_slot) (already the bcps_det_arm's op_zres path).  SN4/Icon watermarks: icn_cells_graph and pl_cells_graph are mutually exclusive by construction; the Icon CPS arm (SCRIP_ZD_ICN_CPS) uses n_operands correctly (args ARE spine cells in Icon).  ONE AUTHORITY per s22k law -- this guard and the n_operands arm below are the two sites that name IR_CALL_PROC_STAGED; both must be consulted together. */
    if (op == IR_SUSPEND && g_emit_cfg && g_emit_cfg->pl_cells_graph) return 0;   /* PL-ZK-5 ONE AUTHORITY: IR_SUSPEND on pl_cells_graph has nops=0 for the ZD planner.  Prolog lower_prolog.c builds IR_SUSPEND with ir_operand_push(sp,mk)+ir_operand_push(sp,ab): operands[0]=predicate body graph, operands[1]=clause list graph -- both are generator-graph references, not inline value-spine predecessors in the run.  The ZD arm in bb_suspend.cpp (ZK-2) reads the yield value from ZOPQ(0,0/8) (the predecessor's RSP-FORTH cell); with nops=0 the planner skips the operand-in-run check and zd_plan no longer declines at "opnd op=IR_SUSPEND".  SN4/Icon watermarks: icn_cells_graph and pl_cells_graph are mutually exclusive (R-ZK-A); Icon IR_SUSPEND n_operands IS a value-spine predecessor (yield value) and must keep the n_operands arm; SN4 never emits IR_SUSPEND -- byte-identical by construction.  ONE AUTHORITY per s22k law.  Killswitch: SCRIP_ZD_PL_SUSP=0 in zd_wl_kind disables admission; nops=0 here is a dependent fix, inert when admission is off. */
    return (op == IR_UNOP || op == IR_ASSIGN || op == IR_DEREF || op == IR_COERCE_STRING || op == IR_COERCE_INTEGER || op == IR_FIELD_VAR || op == IR_MATCH_BEGIN || op == IR_RETURN) ? 1 :   /* ⭐ ZK-4 ADDITIVE: IR_RETURN joins the 1-operand arm -- operands[0] is the return expression; staged as ZOPQ(0,0/8) in the ZD arm. ONE AUTHORITY per s22k law. */ (op == IR_BINOP || op == IR_COERCE_NUMERIC || op == IR_CMP_TEST || op == IR_SUBSCRIPT || op == IR_ASSIGN_VAR || op == IR_BINOP_TEST) ? 2 :   /* ⭐ ZK-2 ADDITIVE: IR_BINOP_TEST joins the 2-operand arm -- lhs=operands[0] rhs=operands[1]; the ZD arm reads ZOPQ(0,0/8) and ZOPQ(1,0/8). Staging loop iterates [0..nops) and stages op_zread[k]=delta_out(test)-delta_out(operand[k]) for each. ONE AUTHORITY per s22k law. */ (op == IR_MATCH_REPLACE) ? 2 : (op == IR_MAKE_LIST || op == IR_CALL || op == IR_CALL_BUILTIN_ICON || op == IR_CALL_BUILTIN_PROLOG || op == IR_CALL_PROC_STAGED || op == IR_SUSPEND || op == IR_TO || op == IR_MATCH_LEN || op == IR_MATCH_ANY || op == IR_MATCH_NOTANY || op == IR_MATCH_POS || op == IR_MATCH_RPOS || op == IR_MATCH_TAB || op == IR_MATCH_RTAB || op == IR_MATCH_SPAN || op == IR_MATCH_BREAK || op == IR_MATCH_BREAKX || op == IR_MATCH_ASSIGN_COND || op == IR_MATCH_ASSIGN_IMM || op == IR_MATCH_VALUE) ? (int)nd->n_operands : 0; }   /* ⭐ ZK-3 ADDITIVE: IR_TO joins the n_operands arm (ZD-ICN-TO s213 fix) -- bb_to.cpp ZD arm reads ZOPQ(0,0/8) = lo and ZOPQ(1,0/8) = hi; without this entry zd_nops returned 0 for IR_TO, staging loop skipped both operands, g_zd_read[0/1] stayed zero, and ZOPQ(k) = [rsp+0] read from TO's own freshly-allocated uninitialized K=32 region instead of the predecessor lit cells (measured: `every i := 1 to 3` printed Error 1: Illegal data type).  n_operands is 2 for the simple `a to b` form (lower_icon.c: ir_operand_push(to,lr); ir_operand_push(to,mr)) and >=2 for IR_TO_BY -- the n_operands arm is the ONE authority, NOT a hardcoded 2.  THE ONE AUTHORITY per the s22k law. */   /* ZD-ICN-CALL (s208): IR_CALL_BUILTIN_ICON joins IR_CALL's nd->n_operands arm -- the planner must stage op_zread[0..nargs-1] for each arg so the ZD arm reads ZOPQ(i,0/8) at the correct displacement.  Without this, nops=0 (default) leaves every op_zread zero and the ZD arm reads [rsp+0+nargs*16] = its OWN K cell rather than the predecessor's cell.  THE ONE AUTHORITY per the s22k law (same comment block applies verbatim). */   /* IR_MATCH_ASSIGN_COND/IMM join nd->n_operands arm (s27): COND/IMM carry n_operands=2 (operands[0]=pattern-node, operands[1]=SAVE-box); the ONE cross-box predecessor in the run is the SAVE at operands[1].  ⛔ STAGING LOOP MUST SPECIAL-CASE COND/IMM: the generic loop stages operands[0..nops-1], which for nops=nd->n_operands=2 would stage BOTH operands[0] (pattern node, NOT a run member) and operands[1] (SAVE, the actual run predecessor) -- wrong for operands[0].  CROSS-FRONT REQUEST to OMEGA s27: the staging loop at emit.cpp:2526 must use operands[1] (index 1) as the sole staged predecessor for COND/IMM.  Consequence: until OMEGA lands the staging fix AND the template ZD arm, SCRIP_ZD_CAP defaults OFF (admission pre-wired, zero behavior change).  IR_MATCH_VALUE joins nd->n_operands arm (s27): VALUE carries n_operands=1, operands[0]=the value-spine pattern DESCR -- that is the sole run predecessor, staged correctly by the generic loop as ZOPQ(0,8). */   /* IR_MATCH_TAB/RTAB/SPAN join nd->n_operands arm (ZD-5b s25): static form n_operands=0, dynamic n_operands=1.  IR_MATCH_REM nops=0 (no arg, default correct).  THE ONE AUTHORITY. */   /* ZD-5b (A-7, s24a): ASSIGN_COND/IMM nops will be 1 (operand[1]=SAVE cross-box read) once OMEGA delivers bb_match_capture ZD arm; ASSIGN_SAVE nops will be 0 (writer, not reader).  Deferred here until the template arm exists. */   /* ZD-5 MATCH-SPINE: HEAD operand[0] = subjval (lower_snobol4.c:1766) read via ZOPQ(0); REPLACE operands [head, rv] -- head is the owner-convention pointer (K=0, its staged zread is never rendered) and rv is the replacement DESCR read via ZOPQ(1); rt_match_replace's rsi/rdx pair is the SUBJECT descr (the OFF-world op_sa spelling reads [___+448] '# sub_lo/sub_hi' -- MEASURED this session: the first armed attempt inverted this, fed the replacement into the subject seats, and roman printed only the last recursion level), so the armed arm reads ZOPQ(2); the REPLACEMENT travels BY ADDRESS via r9 (the op_sb 'replp' slot), so armed r9 becomes a lea of rv's cell, ZOPQ(1). */   /* ZD-7 (b): IR_CALL n_operands = number of args passed; zd_plan's predecessor-in-run check iterates [0..nops) and stages op_zread[k] = delta_out(call) - delta_out(operand[k]) for each.  THIS IS THE ONE AUTHORITY -- the drive-loop staging site at emit.cpp:2346 calls it; adding IR_CALL here means the per-arg ZOPQ differences are staged automatically. */   /* ZD-2d/2e/2f: COERCE_NUMERIC reads operand 0 as the value and operand 1 as the partner type-peek; CMP_TEST takes both coerced operands by address; SUBSCRIPT takes base and index by value.  All stage two op_zread differences.  THIS IS THE ONE AUTHORITY -- the drive-loop staging site calls it rather than re-deriving, since the moment those two disagreed every operand rendered [rsp+0] and 29 programs silently returned wrong answers. */   /* ZD-7 (b): IR_CALL n_operands = number of args passed; zd_plan's predecessor-in-run check iterates [0..nops) and stages op_zread[k] = delta_out(call) - delta_out(operand[k]) for each.  THIS IS THE ONE AUTHORITY -- the drive-loop staging site at emit.cpp:2346 calls it; adding IR_CALL here means the per-arg ZOPQ differences are staged automatically. */   /* ZD-2d/2e/2f: COERCE_NUMERIC reads operand 0 as the value and operand 1 as the partner type-peek; CMP_TEST takes both coerced operands by address; SUBSCRIPT takes base and index by value.  All stage two op_zread differences.  THIS IS THE ONE AUTHORITY -- the drive-loop staging site calls it rather than re-deriving, since the moment those two disagreed every operand rendered [rsp+0] and 29 programs silently returned wrong answers. */
/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int emit_graph_has_deep_arrival(IR_graph_t *g);   /* forward: defined below; referenced by emit_match_begin_stfh_k which must be callable at plan time (before g_emit graph-level fields are set) */
int emit_match_begin_stfh_k_raw(void) {   /* MATCH-RBP (s65e): the graph-qualification body, unchanged — the wrapper below is the ONE AUTHORITY callers keep using *//* ZD-8·STFH-CARVE (FINDING-2026-08-09g): THE ONE AUTHORITY for the MATCH_BEGIN head-carve byte count.  Returns 64 when the OS-2·SLICE-1 GLUE-O deep-main stfh arm fires AND has no bracket leave to reclaim it; 0 otherwise.  Called at BOTH plan time (from zd_k, before g_emit graph-level fields are populated) AND emit time (from the template, after g_emit is fully staged), so all inputs are derived from g_emit_cfg (the IR graph — available at both times).  FLAT_STMT_FRAME DISJUNCT CONTRIBUTES 0: stfh() in the template fires for flat_stmt_frame too, but the STF bracket leave (mov rsp,___ at emit.cpp:841) already reclaims ALL rsp depth — returning 64 there would double-count the carve.  We only return 64 for the OS-2 arm where no bracket leave exists.  JMP-ENTRY FLAGS (flat_jmp_entry/flat_pat/flat_gen/flat_lcl_proc/zframe_graph) are always 0 for any graph containing IR_MATCH_BEGIN: those flags are set only for blob/proc/gen/zframe graphs, which are jmp-entered and never contain a MATCH_BEGIN node.  The absence of MATCH_BEGIN-incompatible opcodes in g_emit_cfg is therefore the sufficient guard.  SCRIP_OS_CAP=0 restores 0 byte-identically (the killswitch for the OS-2·SLICE-1 widening that introduced the stfh arm). */
    { static int _oscap = -1; if (_oscap < 0) { const char * e = getenv("SCRIP_OS_CAP"); _oscap = (e && *e == '0') ? 0 : 1; } if (!_oscap) return 0; }
    if (!g_emit_cfg) return 0;
    if (!emit_graph_has_deep_arrival(g_emit_cfg)) return 0;   /* flat_deep_arrival equivalent — derived from graph IR, valid at plan time and emit time */
    for (int _i = 0; _i < g_emit_cfg->n; _i++) { IR_t * _nd = g_emit_cfg->all[_i]; if (!_nd) continue; if (_nd->op == IR_MATCH_REPLACE || _nd->op == IR_MATCH_FENCE1 || _nd->op == IR_MATCH_ABORT || _nd->op == IR_MATCH_ARBNO) return 0; }   /* has_replace equivalent — same four opcodes as has_replace_l() in bb_match_begin.cpp */
    return 64;
}
int emit_match_rbp(void) {   /* ⭐ MATCH-RBP (s65e, Lon in-chat): THE ONE AUTHORITY for the RBP-framed MATCH protocol.  Lon's ruling: the CAS lifetime is purely LIFO (++/-- on r12 at capture γ/β); the ONE fact MATCH_END cannot recompute is the MARK (captures accumulate across success by SPITBOL's conditional-assignment semantics, N runtime-variable under retries), so MATCH_BEGIN's ENTIRE setup collapses to an RBP frame holding the saved r12 + PATCTX regs + retry cursor + a reserved RESULT slot for the (SUBJ ? PAT) expression value.  Sentinel DELETED on this arm — all three O(N) tag-0 scans (END pop L(10), pump range L(5), rsp-recover L(8)/L(9)) become single movs off rbp, and rbp is depth-free so the CLIMB-RSP-FIX and cas_rsp_mark die too.  QUALIFICATION = exactly the raw stfh graph class (deep-main, no REPLACE/FENCE1/ABORT/ARBNO family — those templates read head slots this frame no longer writes and keep legacy wholesale).  Consulted at plan time (zd_k via the wrapper below) and emit time (both match templates), same discipline as stfh_k.  Killswitch SCRIP_MATCH_RBP=0 restores legacy byte-identically on BOTH sides via the wrapper. */
    { static int _env = -1; if (_env < 0) { const char * e = getenv("SCRIP_MATCH_RBP"); _env = (e && *e == '0') ? 0 : 1; } if (!_env) return 0; }
    return emit_match_begin_stfh_k_raw() == 64;
}
int emit_match_begin_stfh_k(void) { return emit_match_rbp() ? 0 : emit_match_begin_stfh_k_raw(); }   /* MATCH-RBP: under the rbp frame the 64B carve does not exist, so the planner must not account it — zd_k and the M-2 BUG-2 STATEMENT_END formula read 0 through this ONE wrapper and release only the pre-match producers; the frame releases ITSELF via mov rsp,rbp / pop rbp at both exits. */
/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int zd_k(IR_t * nd) { int op = (int)nd->op; if (op == IR_TO) return 32;   /* ZD-ICN-TO (s211): IR_TO K=32 -- result DESCR 16B at ZRES(0/8) + counter 16B at ZLOC(0)=[rsp+16].  A kind whose K is neither 0 nor 16 changes THIS LINE ONLY per the s22k law. */ return (op == IR_ASSIGN || op == IR_GOTO || op == IR_GOTO_DEFERRED || op == IR_SAVE_RESTORE || op == IR_FUNC_ACTIVATE ||   /* AB: self-framing block, K=0 ζ-spine cell -- ONE LINE ONLY per s22k */ op == IR_MATCH_BEGIN || op == IR_MATCH_END || op == IR_MATCH_REPLACE || op == IR_STATEMENT || op == IR_STATEMENT_BEGIN || op == IR_STATEMENT_END || op == IR_MATCH_LIT || op == IR_MATCH_LEN || op == IR_MATCH_ANY || op == IR_MATCH_NOTANY || op == IR_MATCH_POS || op == IR_MATCH_RPOS || op == IR_MATCH_ASSIGN_COND || op == IR_MATCH_ASSIGN_IMM || op == IR_MATCH_VALUE || op == IR_MATCH_ALTERNATE || op == IR_MATCH_FENCE1 || op == IR_BOUND || op == IR_UNMARK || op == IR_CONJUNCTION || op == IR_CUT || op == IR_MOVE_LABEL || op == IR_GLIT || op == IR_GCC || op == IR_GALT || op == IR_RETURN || (op == IR_DISJUNCTION && nd->n_operands == 0) || (op == IR_MATCH_DEFER && nd->pat_static && IR_LIT(nd).sval && !strncmp(IR_LIT(nd).sval, "PATV$", 5))) ? 0 : 16; }   /* PL-ZK-4 ADDITIVE (preserved through M-3 merge): IR_DISJUNCTION (0-operand Prolog form) joins K=0 -- pure control-dispatch, no own result cell; Icon nary form (n_operands>0) stays K=16. ONE LINE ONLY per s22k law. */   /* RK-GRAM ADDITIVE: IR_GLIT/IR_GCC/IR_GALT join K=0 -- Raku grammar wire ops, K=0 transparent (no own result cell). ONE LINE ONLY per s22k law. */   /* ⭐ ZK-4 ADDITIVE: IR_RETURN joins K=0 -- terminal transfer, no own result cell; writes predecessor's DESCR to FRQ(0)/FRQ(8) (the proc result slot) and exits γ. ONE LINE ONLY per s22k law. */   /* PL-ZK-1 ADDITIVE: IR_CUT K=0 (control-only, no result cell) + IR_MOVE_LABEL K=0 (wire, no result cell). ONE LINE ONLY per s22k law. */   /* ⭐ ZK-2 ADDITIVE: IR_CONJUNCTION joins K=0 -- transparent slot-alias, no own 16B cell (the dispatch aliases operand's slot via bb_slot_register; template emits pair_loop only in the alias case). ONE LINE ONLY per s22k law. */ /* ⭐ ZK-3 ADDITIVE: IR_BOUND/IR_UNMARK join K=0 -- transparent bracket pair, no own 16B cell; cells arm uses op_zgpop at UNMARK gamma to release accumulated depth.  ONE LINE ONLY per s22k law. */   /* ⭐ W-2b ONE-AUTHORITY: zd_k and zd_wl_kind must agree for pat_static DEFER nodes.  PATV$ guard: K=0 only for PB-1s pre-chain snapshot DEFERs (PATV$-prefixed hidden globals, never appear inside ARBNO bodies by construction).  Generic pat_static=1 without PATV$ prefix keeps K=16 so ARBNO body scans are not affected by user names that happen to be defer-free. */   /* ⭐ ZD-PATREF (OMEGA s39): IR_MATCH_PATREF(DELETED)/IR_MATCH_DEFER join K=0 -- they are TRANSFER boxes, not value producers.  Both enter the target pattern graph through bb_glue_pass_wires (lea rcx,γ + lea rdx,ω + jmp rax -- the FLAT glue, zero frame) and return through those wires; the match "yields" through scanner registers and capture side effects exactly as IR_MATCH_SEQUENCE/END do, never through a 16B spine cell.  Their op_off reads (FRQ(_.op_off) in bb_match_defer, seal==1 arm) are zls-GRANTED slot reads, not ZRES cell reads, so K=0 does not orphan a write.  THE ONE AUTHORITY per the s22k law -- ONE LINE ONLY. */   /* IR_MATCH_FENCE1 joins K=0 (O-7 s30): FENCE1 yields no result cell; its watermark quad is granted via FRQ (___-relative, depth-free under ZW frame), not a 16B spine cell. */   /* IR_STATEMENT_BEGIN/END join K=0 (s26/ALPHA s30): statement bracket PAIR, K=0 transparent relays like IR_STATEMENT. */   /* IR_MATCH_ALTERNATE joins K=0 (ZD-5b O-7/s29, ALPHA A-8 cross-front): zero-cell envelope -- ALT-FLAT arm uses per-BB frame-relative address slots (FRQ(op_off+8/16)), no own 16B value cell.  THE ONE AUTHORITY per the s22k law -- ONE LINE ONLY. */   /* IR_MATCH_ASSIGN_COND/IMM join K=0 (s27): readers of SAVE's cursor cell, no own result cell -- the length is computed as r14d-ZOPQ(1,8) and written into the pend/CAS entry inline.  IR_MATCH_VALUE joins K=0 (s27): pattern-match side effect (cursor advance) only, no DESCR result cell.  IR_MATCH_ASSIGN_SAVE stays K=16: it stores r14d to own cell ZRES(0) -- the SAVE cursor the paired COND/IMM reads back. */   /* IR_MATCH_ANY/NOTANY/POS/RPOS join K=0 (ZD-5b s25): scanner-register-only (r14d/r15d/r13), no value cell; dynamic args read via ZOPQ(0,8)[+ZOPD(0,4) for charset].  IR_MATCH_LEN joins K=0 s25.  IR_MATCH_LIT joins K=0 s24b.  THE ONE AUTHORITY per the s22k law -- ONE LINE ONLY. */   /* IR_STATEMENT joins the K=0 class s23k slice 0: the bracket box yields no cell; its exits WHACK (gamma K_total / omega depth stubs), never carve. */   /* ZD-5 MATCH-SPINE joins the K=0 class: the quartet's own storage is the statement claim (PATCTX quartet, marks, blob quads) staged by the hybrid arm, never a 16B value cell -- the match "yields" through REPLACE/capture side effects, not a spine cell. */   /* ZD-SR joins the K=0 class: a transparent protocol box yields no cell, exactly like the IR_ASSIGN sink and the IR_GOTO_DEFERRED transfer already listed here.  THE EDIT IS THIS ONE LINE, which is the promise ZD-K1/K2 made. */   /* ZD-8 (b3) THIS IS THE ONE AUTHORITY for K, the alpha-carve byte count of an admitted node: a value-spine sink that yields no cell (IR_ASSIGN, IR_GOTO_DEFERRED) carves 0; every other admitted kind carves 16.  Until s22k this rule was SPELLED TWICE -- zd_plan's depth model and the drive loop's g_zd_k staging each held a copy -- and editing one alone made them disagree by exactly K, a silent stack skew on every node of the admitted kind (MEASURED: segfault, zero output, on 1020_code_label_transfer; it also cost s22j a misdiagnosis of the innocent by-name ZD arm).  Same disease zd_nops carries its own law for, and the s22i note at the staging site predicted this exact fold.  A kind whose K is neither 0 nor 16 changes THIS LINE ONLY. */
static void zd_plan(IR_t **nodes, int n, unsigned char *zon, int *zout, int *zgpop, int *zwpop) {
    extern const char * bb_src_of(const IR_t *);
    static int _zd = -1, _dg = -1; static const char * _zo; static const char * _zs;
    if (_zd < 0) { const char * e = getenv("SCRIP_ZD"); _zd = (e && *e == '0') ? 0 : 1; const char * d = getenv("SCRIP_ZD_DIAG"); _dg = (d && *d == '1') ? 1 : 0; _zo = getenv("SCRIP_ZD_ONLY"); _zs = getenv("SCRIP_ZD_SKIP"); }
    for (int i = 0; i < n; i++) { zon[i] = 0; zout[i] = -1; zgpop[i] = 0; zwpop[i] = 0; }
    { static int _lp=-1; if(_lp<0){const char*e=getenv("SCRIP_ZDLOCAL");_lp=(e&&*e=='1')?1:0;} if(_lp){ extern int is_global(const char *); for (int i=0;i<n;i++){ int o=(int)nodes[i]->op; if(o==IR_VAR||o==IR_ASSIGN){ const char*vn=IR_LIT(nodes[i]).sval; int gl = (vn && is_global(vn) && !graph_has_local(g_emit_cfg, vn)); if(!gl) fprintf(stderr,"[ZDLOCAL] %s name=%s pinned=%d fbdata=%d\n", bb_op_name(nodes[i]->op), vn?vn:"<null>", x86_fb_pinned(), x86_fb_data()); } } } }
    { /* _ng gate deleted, CARVE-DATA-ERAD s23a */
      /* ZD-NOGRAPH (Lon s21x-w "NO FUNCTION-level processing for ZETA whatsoever.  ONLY statement level scoping"): the jmp-entry conjunct below is a
       * WHOLE-GRAPH early return -- one pinned graph declines every statement in it before any per-node arm is consulted (measured s204, queens:
       * 10/10 graphs deep=1 stub_ok=0).  Runs are already rooted at bb_src_of statement heads, so statement scoping is intact without it; this gate
       * removes the function-level veto and lets the per-run gate speak for itself. */
      if (!_zd || x86_port_mode() != ZC_PORT_FORTH || n <= 0) return; }
    { if (g_emit_cfg && g_emit_cfg->icn_cells_graph) { for (int _bi = 0; _bi < n; _bi++) { IR_t * _bgt = zd_chase(nodes[_bi]->γ.node); IR_t * _bot = zd_chase(nodes[_bi]->ω.node); for (int _bj = 0; _bj <= _bi; _bj++) { if (nodes[_bj] == _bgt || nodes[_bj] == _bot) return; } } } }   /* ⭐ ZK-2 GRAPH-WIDE BACK-EDGE DECLINE (s226): a chased γ/ω edge from nodes[_bi] landing on nodes[_bj] with _bj<=_bi (nodes[] is execution order) is a loop back-edge.  The per-run guard below cannot see it when the edge crosses run boundaries (while/until: body-run ASSIGN.γ→GOTO→condition-run VAR).  A back-edge join is reached at two RSP depths (0 on first entry vs ΣK after one lap of carves), so ZOPQ offsets staged for the first depth read wrong memory on lap 2+ (MEASURED: until_minimal printed 5 not 4 under the mixed per-statement regime; countdown wrote 1..5 not 1..3).  Early return leaves zon[] all-zero — the existing self-decline pattern ("zd_plan self-declines jmp-entry/non-FORTH, leaving zon all-zero") — so the whole graph takes the non-ZD FRQ arm, behaviorally the working UCLAIM path.  CONSERVATIVE BY DESIGN: declines every statement in a loop-containing graph, including straight-line ones; ZK-6's suspension mechanism owns per-lap release and will narrow this.  zd_chase both edges: the back-edge's last hop is a GOTO wire, so a raw-edge compare misses it.  _bj<=_bi includes self-loops.  GATED icn_cells_graph: SN4/Prolog byte-identical by construction.  s22k law: no K spelled here. */   /* ⭐ CARVE-DATA-ERAD (s23a): the jmp-entry WHOLE-GRAPH veto is DELETED -- blob statements enter the claim regime unconditionally.  zd_stub_ok is caller-less pending janitorial deletion. */
    int * claim = (int *)alloca(sizeof(int) * (size_t)n); int * run = (int *)alloca(sizeof(int) * (size_t)n); int * rpos = (int *)alloca(sizeof(int) * (size_t)n);
    unsigned char * cm = (unsigned char *)alloca((size_t)n); int * wl = (int *)alloca(sizeof(int) * (size_t)n);   /* ⭐ ZD-4 (s23q, ALPHA A-1): the run-closure scratch joins the other PER-CHAIN scratch arrays here.  It previously alloca'd INSIDE the run loop, which was already a frame-growth-per-iteration shape and would have become one per DECLINED run once the walk hoisted above the admission gate -- allocate once, clear per run, same lifetime discipline claim/run/rpos already keep.  n > 0 is guaranteed by the early return above. */
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
        static int _z5b = -1; if (_z5b < 0) { const char * e = getenv("SCRIP_ZD_5B"); _z5b = (e && *e == '0') ? 0 : 1; }   /* ⭐ ZD-5b SUBTREE DESCENT (A-7, s24a): DESIGN DELIVERED, PLANNER WALK BLOCKED ON BLOB-CLOSURE ORDERING.  The run walker must extend from linear γ-chain to γ-chain + subtree descent through ALTERNATE/SEQUENCE operand pairs into arm-interior leaf kinds.  BLOCKER (s24a measurement): a naive phase-2 walk (descend arm entries AFTER the main γ-chain loop) claims arm-interior nodes speculatively, before the blob closure (cm[]) is computed; this steals nodes from OTHER statements' γ-chain walks and corrupts their runs.  REQUIRED FIX (next session): hoist the blob closure computation ABOVE the run walker, use cm[] to gate phase-2 so it only claims nodes inside THIS statement's blob.  OR: change the walk to be a SINGLE PASS that computes closure on-the-fly, gating arm-entry descent on whether the entry is already a cm[] member.  SCRIP_ZD_5B=0 is a no-op killswitch (walk below is the unchanged prior linear walk; the env var is pre-cached for the real landing). */
        while (cur && guard++ <= n) {   /* LINEAR γ-CHAIN WALK -- unchanged from prior; subtree descent deferred per blocker above */
            int ci = -1; for (int k = 0; k < n; k++) if (nodes[k] == cur) { ci = k; break; }
            if (ci < 0 || claim[ci] >= 0) break;
            if (rl > 0 && bb_src_of(nodes[ci])) break;
            run[rl] = ci; rpos[ci] = rl; claim[ci] = hi; rl++;
            cur = zd_chase(cur->γ.node);
        }
        for (int k = 0; k < n; k++) cm[k] = 0;
        int nblob = 0;
        { int wn = 0;
          for (int r = 0; r < rl; r++) { cm[run[r]] = 1; wl[wn++] = run[r]; }
          while (wn > 0) { int i = wl[--wn];
              for (int a = 0; a < nodes[i]->n_operands; a++) { IR_t * p = nodes[i]->operands[a]; if (!p) continue;
                  for (int k = 0; k < n; k++) if (nodes[k] == p && !cm[k]) { cm[k] = 1; wl[wn++] = k; nblob++; break; } } } }   /* ⭐ ZD-4 (s23q, ALPHA A-1): the run's OPERAND CLOSURE (members + their operand trees = the blob) is HOISTED here, above the dynamic-box veto, so that ONE closure walk serves both the veto's scope test and the hybrid-claim span below.  It was computed inside `if (ok)` and therefore could not scope an admission verdict that runs before it; nothing about the walk itself changed (same seed, same worklist, same nblob), only WHERE it stands. */
        int ok = (rl > 0); const char * why = ""; int badi = -1; int rgood = rl;
        for (int r = 0; r < rl && ok; r++) { int i = run[r];
            { int _a = zd_wl_kind(nodes[i]);
              if (!_a) { ok = 0; why = bb_op_name(nodes[i]->op); badi = i; rgood = r; break; } }
            if ((_zo && !zw_nid_listed(_zo, i)) || (_zs && zw_nid_listed(_zs, i))) { ok = 0; why = "nidgate"; badi = i; rgood = r; break; }
            { int no = zd_nops(nodes[i]); if (nodes[i]->n_operands < no) { ok = 0; why = "nops"; badi = i; rgood = r; break; }
              for (int j = 0; j < no; j++) { IR_t * p = nodes[i]->operands[j]; int f = -1; for (int k = 0; k < rl; k++) if (nodes[run[k]] == p) { f = k; break; }
                  if (f < 0 || f >= r) { ok = 0; why = "opnd"; badi = i; rgood = r; break; } } }
        }
        { if (ok && g_emit_cfg && g_emit_cfg->icn_cells_graph) { for (int _rb = 0; _rb < rl && ok; _rb++) { IR_t *_gt = zd_chase(nodes[run[_rb]]->γ.node); if (!_gt) continue; for (int _rg = 0; _rg < _rb && ok; _rg++) { if (nodes[run[_rg]] == _gt) { ok = 0; why = ir_is_generator_kind(nodes[run[_rg]]->op) ? "gen-loop-body" : "loop-backredge"; badi = run[_rg]; rgood = _rg; } } } } }   /* ⭐ ZK-2 ZD-LOOP-BACKREDGE DECLINE (s226, generalizes ZD-GEN-LOOP-BODY): any backward γ edge in the run declines the cells arm on icn_cells_graph.  The prior guard required ir_is_generator_kind at the target; while/until loop back-edges land on non-generator IR_VAR (the loop variable) which escaped the check and caused wrong output.  WALL MECHANISM: the join node (IR_VAR) is reached from two paths with unequal depths — initial (depth=0) vs loop-back (depth=K_total after one iteration's carves).  ZOPQ offsets are computed from the initial path's depth; on the second+ iteration the cells are K_total bytes deeper than expected, so ZOPQ reads the wrong memory.  BOTH cases (generator-kind and non-generator) share the same root cause: the ZD non-popping spine model assumes a single linear arrival depth per node (DAG semantics) which back-edges violate.  ZK-6 (suspension mechanism) and explicit BOUND/UNMARK brackets in the lowerer are the correct rung for full loop support.  why string distinguishes cases for census readability.  GATED icn_cells_graph: SN4/Prolog byte-identical.  s22k law: no K constant spelled here. */
        (void)rgood;   /* ⛔ ZD-PREFIX WAS TRIED AND IS UNSOUND -- MEASURED, DO NOT RE-DERIVE IT.  The tempting relaxation of the ALL-OR-NOTHING gate
         * above is to arm the MAXIMAL PASSING PREFIX of the run instead of declining the whole statement (the Icon census reads armed=0/declined=271
         * precisely because one unarmable node vetoes every BB in its statement).  The argument for it is that a declined node's result is not
         * cell-resident so later consumers must decline too, making arming a PREFIX property; and that the boundary node's gamma/omega targets are
         * outside the armed set, so the existing !gin/!oin arms already release before control reaches unarmed code.  THAT ARGUMENT IS WRONG AND THE
         * COUNTEREXAMPLE IS TWO LINES: `x := 1 + 2; write(x)` prints NOTHING under prefix arming (baseline prints 3), and `every i := 1 to 3 do
         * write(i)` prints 0 (baseline 1 2 3) -- SILENT WRONG ANSWERS at rc=0, the worst class, and note the first has no backtracking at all, so
         * resume-into-released-storage is NOT the mechanism.  ROOT CAUSE: the operand loop above checks only that an armed node's OPERANDS are
         * earlier armed nodes.  It never checks the CONVERSE -- that an armed node's CONSUMERS are armed.  An armed producer writes its rsp cell
         * while an unarmed consumer reads the flat frame slot, and truncating a prefix manufactures exactly that disagreement at the boundary.  The
         * all-or-nothing gate is therefore not timidity: it is what makes the producer/consumer STORAGE REGIME agreement hold across a whole run.
         * Any future partial arming must carry a consumer-side check, i.e. arm a CONVEX region closed under both operand and consumer edges, not a
         * prefix.  Suite: 238/25/30 -> 78/185/30 under prefix arming. */
        if (ok) { int zd = 0; int zdh_match = -1;   /* M-2 BUG-2 FIX: zdh_match = zd at MATCH_BEGIN entry (= depth of pre-match producer cells only); used as gpop at STATEMENT_END when hfc because the cas_rsp_mark restore collapses all in-bracket cells (pattern-arg cells like LIT_INTEGER), leaving only the pre-match producers for STATEMENT_END to release.  -1 = no hfc MATCH_BEGIN seen yet. */
            /* ⭐ ZD-5 MATCH-SPINE (this session, Lon "complete the NON-POPPING FORTH-style RSP ZETA stack"): an ARMED run may now carry a match statement, whose blob hangs off run members as
             * OPERAND TREES exactly as the declined path's OPERAND CLOSURE comment records.  Two consequences, both fatal to the s22s-b trio-transparent attempt and both repaired here at the
             * staging site the addendum named suspect #1: (a) EXIT TEST -- a spine edge landing on a blob β is intra-statement, never a release site, so gin/oin must consult the FULL closure
             * (any position, the declined path's own law) whenever the closure is wider than the run; the k>r positional test is kept VERBATIM for closure==run so every currently-armed value
             * run stages byte-identically.  (b) HYBRID CLAIM -- the blob's flat zls quads (and the head's PATCTX/mark quads) still need their reservation when the run arms, so the run head
             * stages a claim over the closure span exactly as the declined arm does; armed members' cells ride BELOW it (the hook carves claim before cell, cell stays TOS), and each member's
             * zud carries Kc + its depth-below-claim so zvo_resolve's (dout_reader - dout_owner) term rebases every FRQ spelling by the cells standing between it and the claim base.  Blob
             * members (non-run closure) execute inside the match at the HEAD's depth, so they carry the head's zud.  Terminal pops grow by Kc: the CAS-marker fail exit first restores rsp to
             * the head-α mark (claim_base - zd_before_head), then the staged wpop frees zd_before_head + Kc, landing exactly at statement entry -- the FORTH bulk unwind and the one release
             * authority composing, no hand-counted pops. */
            for (int r = 0; r < rl; r++) { int i = run[r];
                int K = zd_k(nodes[i]);   /* ZD-8 (b1) sink -- K comes from zd_k, THE ONE AUTHORITY (defined beside zd_nops).  Do not re-spell the rule here. */
                { if (nodes[i]->op == IR_MATCH_BEGIN && zdh_match < 0) zdh_match = (int)zd + K; }   /* M-2 BUG-3: record pre-head depth for ALL non-mech2 MATCH_BEGINs (not just hfc). MATCH_END's mov rsp,[___+64] (zls2_mark restore) collapses in-bracket cells for every non-mech2 graph; STATEMENT_END must release the post-MATCH_BEGIN rsp depth (= pre-match producers + K(MATCH_BEGIN)) because zls2_mark is saved AFTER the stfh carve (bb_match_begin line 68-71), so the zls2_mark restore leaves the stfh bytes still on the stack. ZD-8·STFH-CARVE (FINDING-2026-08-09g): when K=0 (the historic case), zd+K==zd, byte-identical. When K=64 (OS-2 GLUE-O disjunct), STATEMENT_END must release 64 extra bytes beyond the pre-match producers; recording zd+K here is THE ONE AUTHORITY for that delta -- neither the template nor zd_k re-spell it. Mech2 MATCH_BEGINs do not enter armed ZD runs (their whack is framed_leave), so no mech2 case is reached here. */
                zon[i] = 1; zout[i] = zd + K; zd = zd + K;
                IR_t * gt = zd_chase(nodes[i]->γ.node); IR_t * ot = zd_chase(nodes[i]->ω.node);
                int gin = 0; int oin = 0;
                if (nblob > 0) { for (int k = 0; k < n; k++) if (cm[k]) { if (nodes[k] == gt) gin = 1; if (nodes[k] == ot && !(port_sz_beta(nodes[i]->ω.sz) && beta_is_stmt_land(ot))) oin = 1; } }   /* fA (this session): a β-edge into the OWN STATEMENT_BEGIN is a statement EXIT through the landing pad, never intra -- without this conjunct the own-sbeg membership set oin=1 and suppressed the exit residual: MEASURED 16B/failure leak, 500k-failure witness dies at default stack while pristine passes (w_leak.sno).  Mirrors the 2070 arm; the R1 comment promised this conjunct at "the suppression sites (planner oin)" -- the membership arms never received it because fB's anticipated chase lands on the TARGET statement (outside own run) and never needed it. */
                else for (int k = 0; k < rl; k++) { if (nodes[run[k]] == gt && k > r) gin = 1; if (nodes[run[k]] == ot && !(port_sz_beta(nodes[i]->ω.sz) && beta_is_stmt_land(ot))) oin = 1; }   /* ⭐ ZD-5b-SAVE-OIN (s14): drop k>r on oin — scan-retry back-edge (ω→MATCH_BEGIN, k<r) is intra-match; forward-only guard was correct for gin but wrong for oin. */
                { if (!gin && gt && gt->op == IR_SUCCEED && g_emit_cfg && g_emit_cfg->icn_cells_graph && port_sz_beta(nodes[i]->ω.sz)) gin = 1; }   /* ZK-3 ADDITIVE (SUCCEED-GIN): on the cells arm, a gamma arc terminating at IR_SUCCEED is intra-statement by the non-popping FORTH spine law ONLY when the omega port carries the beta-tag (0xce 0xb2) — the every-body topology (ir_a_Every: CALL_BUILTIN γ→SUCCEED, ω→TO.β). Setting zgpop on that arc fires add rsp,K_total inside the generator loop, freeing TO counter and limits — IR_TO.beta then reads freed stack (SEGV). ZK-4 SLICE 3 ADDITIVE CONJUNCT: port_sz_beta(nodes[i]->ω.sz) — the top-level write(42) topology has γ→SUCCEED but ω→FAIL (not a beta port), so SUCCEED-GIN must not suppress the terminal release there. Without this conjunct, zgpop[CALL_BUILTIN]=0 for write(42) on the cells arm; the statement-terminal γ jmp emits no add rsp,K_total; in m3 the outer RX machinery sees RSP 32B short and SEGVs after correct output (rc=139). In m4 exit() ignores RSP so the bug was invisible. ONE-LINE ONLY per s22k law. GATED icn_cells_graph: SN4/Prolog byte-identical by construction (icn_cells_graph=0). */
                { (void)0; }   /* ZK-2 GEN-BACK-GIN SCOPE BOUNDARY (this rung, measured and scoped): setting gin=1 for backward γ edges to IR_TO (the every-loop-body back-edge IR_ASSIGN.γ→IR_TO.β) would suppress gpop[IR_ASSIGN]=144, keeping downstream cells (COERCE/BINOP) live across iterations. BUT downstream nodes RE-EXECUTE on every β pass (they are deterministic, not suspended) — their cells accumulate per iteration, shifting ZRES and all ZOPQ reads further down the stack each pass. The topology is incompatible with the ZD spine model's non-popping assumption for re-executing nodes. CORRECT SCOPE for this rung's ZD arm: IR_TO in the leaf-spine class (write(1 to 3), fib generator calls) where the γ chain terminates at CALL_BUILTIN→SUCCEED (handled by SUCCEED-GIN, line 2054) without a computation chain that re-executes. The every-total-sum topology (every total:=total+(1 to n)) DECLINES the cells arm by the all-or-nothing gate — wrong output, not SEGV, deferred to ZK-6 where the suspended-state architecture decides. ZK-6 Option P (pending cells on one spine) may revisit this boundary. */
                { int oib = port_sz_beta(nodes[i]->ω.sz); if (!oin && oib && K == 0 && !beta_is_stmt_land(ot)) oin = 1; }   /* ZD-5b (s24b): blob-interior K=0 leaf kinds (LIT/LEN/ANY/NOTANY/SPAN/TAB/RTAB/POS/RPOS/REM) wire omega to MATCH_BEGIN's BETA (scanner retry, port tag 0xce 0xb2); beta_is_stmt_land exempts STATEMENT_BEGIN.β (landing pad). */
                { if (!oin && K == 0) { int _io = !ot; if (!_io) { for (int _ik = 0; _ik <= r; _ik++) { if (nodes[run[_ik]] == ot) { _io = 1; break; } } } if (_io) oin = 1; } }   /* ⭐ M-1 FIX 3 + ZWR EXTENSION (C-5 CLIMB, ZK-3/ZK-4 regression fix): backward-edge oin guard for ZWS and ZWR.  !ot arm: ω.node=NULL means template-routed fail (e.g. MATCH_LIT abort-fail path) — MATCH_END is sole release authority, suppress wpop.  ot-found arm: ω points to an earlier run member (k<=r backward edge) — same suppression law as ZWS M-1 FIX 3.  GATED K==0 && (zws||zwr): non-match, non-window, K>0 runs are byte-identical.  ONE LINE per s22k law. */
                                { long kc = 0;
                int _wzdepth = (int)zd;   /* ⭐ ZW-12 HEAD-WPOP FIX: MATCH_BEGIN's ω exit is the pattern-exhausted fail — it fires AFTER the whack restores rsp to the claim base, so it sees only pre-frame depth (zdh), NOT the post-ZW_FRAME_TOTAL depth (zd which has been incremented by 64).  Using post-ZW zd here inflated wpop by ZW_FRAME_TOTAL=64, causing add rsp,N to release 64 bytes into the caller's frame on each scan retry (rip=0x1/0x3 class, Z4 span crash). */
                { /* ⭐ W-1 FIX: _ms (mech-2 STATEMENT_END/STATEMENT zeroing) REMOVED.  After the _fci correction above, zd at STATEMENT_END correctly reflects only the pre-match producer cells (e.g. the VAR's 16B).  Zeroing zgpop here would suppress that correct release, leaving rsp 16B below call_rsp at program exit → SEGV at ret.  The mech2_whack only restores to α (MATCH_BEGIN entry, which includes producer cells); STATEMENT_END releases the remaining producers above α.  Gate-OFF byte-identical (zwr=0 outside mech-2). */
                if (!gin) zgpop[i] = ((zdh_match >= 0 && (nodes[i]->op == IR_STATEMENT_END || nodes[i]->op == IR_STATEMENT)) ? zdh_match + emit_match_begin_stfh_k() : _wzdepth) + (int)kc;   /* M-2 BUG-2: for STATEMENT_END/STATEMENT in an hfc match run, use zdh_match (pre-match producers) instead of _wzdepth (full including in-bracket cells). cas_rsp_mark restore collapses all in-bracket cells; STATEMENT_END must release only the pre-match producers PLUS the stfh 64B carve. ZD-8·STFH-CARVE (FINDING-2026-08-09g): emit_match_begin_stfh_k() is the ONE AUTHORITY (graph-based, valid at plan time). When the OS-2·SLICE-1 stfh arm fires, MATCH_BEGIN carves 64B at alpha BEFORE cas_rsp_mark is saved; MATCH_END's zls2_mark restore (HKM=[___-56]) brings rsp back to the post-stfh point; the stfh bytes remain on the stack below that. STATEMENT_END must therefore release zdh_match (producers) + 64 (stfh). When stfh does not fire, emit_match_begin_stfh_k()==0 and the formula is byte-identical to the original. */
                if (!oin) zwpop[i] = _wzdepth - K + (int)kc; } }   /* ZD-K2; W-1 FIX: _ms zero removed for wpop too -- after _fci correction the depth is correct, no over-release. Two closing braces: W-1-FIX block + kc/_wzdepth block. */
                if (_dg) fprintf(stderr, "[ZD] h=%d r=%d i=%d %s K=%d zout=%d gpop=%d wpop=%d\n", hi, r, i, bb_op_name(nodes[i]->op), K, zout[i], zgpop[i], zwpop[i]);
            }
        }
        else {
            for (int r = 0; r < rl; r++) rpos[run[r]] = -1;
               if (_dg && rl > 0) fprintf(stderr, "[ZD] run h=%d len=%d DECLINED at i=%d (%s op=%d)\n", hi, rl, badi, why, badi >= 0 ? (int)nodes[badi]->op : -1);
               { static int _gp = -1; if (_gp < 0) { const char * _e = getenv("SCRIP_ZD_GAP"); _gp = (_e && _e[0] == '1') ? 1 : 0; } if (_gp && rl > 0) { for (int _r = 0; _r < rl; _r++) { IR_t * _n = nodes[run[_r]]; const char * _nm = bb_op_name(_n->op); { int _o = (int)_n->op; int _ic = (_o == (int)IR_CALL || (_o != (int)IR_CALL_VALUE && ir_is_call_kind((IR_e)_o))); const char * _cs = (_ic && IR_LIT(_n).sval) ? IR_LIT(_n).sval : "-"; fprintf(stderr, "[ZD-GAP] r=%d op=%s(%d) admit=%d nops=%d callee=%s\n", _r, _nm ? _nm : "<unnamed>", _o, zd_wl_kind(_n) ? 1 : 0, (int)_n->n_operands, _cs); } } } }   /* ZD-PL-0 ADMISSION-GAP CENSUS (this session): the [ZD] line above reports only the FIRST blocker, which cannot size the gap -- an all-or-nothing run declines on node 0 and says nothing about the other rl-1 kinds that would ALSO have to arm.  Prolog measured 0 armed nodes / 1137 graphs, so first-blocker histograms report IR_CALL_BUILTIN_PROLOG forever while the real question is which SET of templates needs ZD arms.  This walks the whole run and prints each node's zd_wl_kind verdict, making the gap a ranked list instead of a repeated headline.  File-static env cache, zero g_* writes, byte-identical inert when off (same shape as the SCRIP_CALL_DIAG probe below). */
               { static int _cd = -1; if (_cd < 0) { const char *_e = getenv("SCRIP_CALL_DIAG"); _cd = (_e && _e[0] == '1') ? 1 : 0; }   /* ZD-7 TAG-SAFE CALLEE CENSUS: partition declined-run first blockers by kind; tag-safe: IR_CALL_VALUE excluded (no sval); IR_SAVE_RESTORE excluded (ival=role, not a pointer); all other ir_norm_call_kind==IR_CALL sub-kinds write sval in every lowerer. */
                 if (_cd && rl > 0 && badi >= 0) { IR_t * _bn = nodes[badi]; int _bop = (int)_bn->op; int _is_call = (_bop == (int)IR_CALL || (_bop != (int)IR_CALL_VALUE && ir_is_call_kind((IR_e)_bop))); const char * _callee = (_is_call && IR_LIT(_bn).sval) ? IR_LIT(_bn).sval : (const char *)0; fprintf(stderr, "[CALL-DIAG] DECLINE blocker op=%s(%d) callee=%s rl=%d badi=%d narg=%d\n", bb_op_name(_bn->op), _bop, _callee ? _callee : "-", rl, badi, _is_call ? (int)_bn->n_operands : -1); } } }
    }
}
/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ⭐⭐⭐ ZD-DEPTH BRICK-WALL CENSUS (this session, Lon directive 2026-08-03c: "continue this for every box and every construct until you hit a BRICK WALL and realize, oh I need a register ___ stable base
 * pointer for what I'm doing").  THE CLAIM THIS MEASURES: the brick wall is not a CONSTRUCT, it is a GRAPH PROPERTY.  Sliding rsp offsets are well defined exactly when every box has ONE static entry
 * depth; the wall is a JOIN whose predecessors arrive at DIFFERENT accumulated depths, because no [rsp+k] spelling can name the same cell down both edges.  The standing list of four ___ constructs
 * (STATEMENT/FUNCTION/ARBNO/FENCE1, s21x-c law 4) should then be the OUTPUT of this test, not its input -- and if the measured set is wider, the list was never the rule, it was a sample of the rule.
 * ARRIVAL-DEPTH MODEL, taken verbatim from the UNWIND four-clause law (HQ 2026-08-03b) so the census cannot drift from the emission it audits: a gamma edge SUSPENDS the box's own cell (clause 1, roll up
 * forward) so it arrives at zout[i] = depth AFTER i; an omega edge FREES the box's own K first (clause 2, "box N's omega frees N's OWN K only") so it arrives at zout[i]-K.  Two edges into one target with
 * unequal arrival depth = WALL.  READ-ONLY BY CONSTRUCTION: no g_emit write, no emission, no zd_* array mutation -- it consumes zd_plan's finished zon[]/zout[] and prints.  Inert unless SCRIP_ZD_DEPTH=1,
 * so the default build is byte-identical and this can ride any rung without gating it.  DECLINED nodes (zon=0) are SKIPPED, not assumed depth 0: their depth lives in the UCLAIM wholesale claim, which is
 * precisely the regime this census exists to help retire -- counting them as 0 would manufacture disagreements that are artifacts of the claim, not of the graph. */
typedef struct { IR_t * t; int d0; int nd; int multi; } zdw_ent_t;
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
                fprintf(stderr, "[ZD-DEPTH]   pred %s %-24s depth=%d%s\n", p == 0 ? "\xce\xb3" : "\xcf\x89", bb_op_name(nodes[i]->op) ? bb_op_name(nodes[i]->op) : "<unnamed>", d, d == w[k].d0 ? "" : "   <-- DISAGREES"); } } }   /* ⭐ ATTRIBUTION (same session, before anyone acts on this census): naming the WALL without naming WHO disagrees is the [ZD-GAP] blindness verbatim -- that diag printed the per-node capability verdict and no run-level cause, which is exactly how the inherited A-1 rung came to name a jmp-entry gate s23a had already deleted, and the fix then was the [ZD-DYN] line naming the vetoing node.  A depth census that reports only "preds=5 first_depth=0" is one rung away from the same misattribution: the actionable fact is WHICH edge arrives wrong and through WHICH port, because that is the edge whose emission has to change.  Second pass, not extra storage -- the wall set is tiny (1410 of 12510 joins) so re-walking edges for wall targets only is free. */
    fprintf(stderr, "[ZD-DEPTH] %s n=%d armed=%d in_edges=%d joins=%d walls=%d\n", prefix ? prefix : "?", n, armed, edges, wn, walls);
    free(w);
}
/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void flat_beta_used_scan(IR_t **nodes, int n, unsigned char *used) {
    for (int j = 0; j < n; j++) if (fc_seq_on(nodes[j]) || fc_alt_active(nodes[j])) { for (int k = 0; k < n; k++) used[k] = 1; return; }
    for (int k = 0; k < n; k++) {
        int op = (int)nodes[k]->op;
        used[k] = (ir_is_generator_kind(nodes[k]->op) || op == IR_SUSPEND || op == IR_CALL || op == IR_CALL_PROC_STAGED || op == IR_CALL_BUILTIN_GEN || op == IR_PROC_GEN || op == IR_REPALT
                   || op == IR_LIMIT || op == IR_GOTO || op == IR_STATEMENT_BEGIN
                   || (g_emit_cfg && nodes[k] == g_emit_cfg->body_root)) ? 1 : 0;   /* STMT-BETA (this session, Lon ruling 2026-08-06): IR_STATEMENT_BEGIN beta is always-live -- it is the named failure landing for the statement scope, the STATEMENT-FAILURE LAW first-class label.  Without this, bused[i]==0 elides the label via op_beta_dead=1 and no statement_begin_β ever appears in emitted .s.  Routing: beta is currently a trampoline (beta: jmp omega = next_stmt_alpha) which is the correct failure semantics -- match_begin exhaust and element omega edges arrive here and fall through to the F-goto.  Future lower rung will route these edges directly to beta rather than bypassing it. */
    }
    for (int j = 0; j < n; j++) {
        int op = (int)nodes[j]->op;
        if (op == IR_MATCH_ALTERNATE || op == IR_MATCH_ARBNO || op == IR_MATCH_FENCE1 || op == IR_SCAN_SEQUENCE || op == IR_SCAN_ALTERNATE || op == IR_DISJUNCTION || op == IR_REPALT)
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int codegen_flat_chain_body(IR_t *entry, const char *prefix) {
    bb_label_t lbl_α, lbl_α_body, lbl_γ, lbl_ω, lbl_β, lbl_res;
    emit_label_initf(&lbl_α,      "%s_α",      prefix);
    emit_label_initf(&lbl_α_body, "%s_α_body", prefix);
    emit_label_initf(&lbl_γ,       "%s_γ",      prefix);
    emit_label_initf(&lbl_ω,       "%s_ω",      prefix);
    emit_label_initf(&lbl_β,       "%s_β",       prefix);
    emit_label_initf(&lbl_res,     "%s_res",     prefix);
    int bare = g_emit.flat_bare_chain;   /* BARE-CHAIN (Lon s62): the six-label proc_* wrapper family + port stubs are the pre-floater chain protocol — for SN4 flat proc chains the body statements exit through the RETURN/FRETURN floaters and the ports never fire, so the wrapper is dead weight in the .s.  LBL__ body chains bind exactly ONE label, <FN>_body (the shim's fold target, computed-goto landings, and the LBL__ registration all repoint to it); DEFINE stub chains keep only proc_<FN>_α (the goto-transfer entry rt_define_site registers — Lon kept it off the removal list).  Dangling chain-level γ/ω/β/res names alias to the floaters: for a DEFINE body, falling off the end IS a RETURN and a chain-level failure IS an FRETURN (SPITBOL Ch.8), so the aliases are not a hack but the true semantics; every such site is post-unconditional-jmp dead code regardless.  m4-only by the driver's setting (the role-4 shim and fold arm are MEDIUM_TEXT this seat — same owed m3 class as s58 tiny). */
    if (bare) { if (strncmp(prefix, "proc_LBL__", 10) == 0) emit_label_initf(&lbl_α, "%s_body", prefix + 10); emit_label_initf(&lbl_γ, "main_\xcf\x89"); emit_label_initf(&lbl_ω, "main_\xcf\x89"); emit_label_initf(&lbl_β, "main_\xcf\x89"); emit_label_initf(&lbl_res, "main_\xcf\x89"); }   /* s62b: the dangling-port aliases MUST name a label every program defines — RETURN/FRETURN floaters are seeded only when the graph's own edges reference them (MEASURED: 1014_func_freturn's body touches only FRETURN, so the shim chain's dead `jmp RETURN` fallback was an undefined reference and the link failed).  main_ω is unconditional and is the correct landing for a port that can only fire on protocol violation: exit 1. */
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
    int entry_is_own_graph_root = (g_emit_cfg && (entry == g_emit_cfg->entry)) ? 1 : 0;   /* EARN-DIV-1: captured BEFORE the relay chase below, and compared against BOTH the raw and chased forms at the group-root guard (see its comment).  Must be sampled here: the chase rewrites `entry` in place, so a test written at the guard cannot tell "this call IS the graph's own root emission" from "this call is a proc inside the graph whose chased entry happens to be some interior node". */
    { int guard = 0; while (entry && (entry->op == IR_SUCCEED || entry->op == IR_FAIL || entry->op == IR_GOTO) && entry->γ.node && guard++ < CH_MAX) entry = entry->γ.node; }   /* HQ s26 (Lon: no PROC-level processing in SNOBOL4): IR_GOTO joins the transparent-relay entry chase -- a chain bound at a label-landing goto (LBL__ pseudo-procs) binds α at the landing's TARGET, so the gate box is never collected and its `sub rsp` / trampoline never emit.  Same law as the s24b zd_wl_kind admission: a pure relay carries no cell and must not shape the chain. */
    if (g_emit_cfg && entry == g_emit_cfg->entry) entry_is_own_graph_root = 1;   /* EARN-DIV-1: the graph's own root emission may itself arrive pre-chased (main's own emit_chain passes g->entry directly and the chase is a no-op for it) -- accept either form, so this fix cannot narrow the pull-in for the citizen it legitimately serves. */
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
#define RPO_PUSH(p)    do { if ((p) && (p)->op != IR_SUCCEED && (p)->op != IR_FAIL && !emit_floater_kind(p) && !(_stmt_seed && (p)->op == IR_STATEMENT_BEGIN) && !RPO_VISITED(p) && qt < Q_MAX) queue[qt++] = (IR_t *)(p); } while (0)   /* FLOATERS: never pulled into a chain's flow — seeded once as the graph-root call's final root blocks below */   /* STATEMENT-ORDER (Lon s62): under _stmt_seed every IR_STATEMENT_BEGIN is its own root block seeded in all[] source order below — successor pushes refuse them so a statement's interior collection stops at the next statement's door */
#define RPO_PUSH_SUCCS(c) \
        /* γ FIRST = popped LAST = finishes last = immediately follows (c) after the reversal */ \
        RPO_PUSH((c)->γ.node); \
        if (((c)->op == IR_MATCH_ALTERNATE || (c)->op == IR_MATCH_ARBNO || (c)->op == IR_MATCH_FENCE1 || (c)->op == IR_SCAN_SEQUENCE || (c)->op == IR_SCAN_ALTERNATE || (c)->op == IR_DISJUNCTION || (c)->op == IR_GALT) && (c)->n_operands > 0) for (int _oi = (c)->n_operands - 1; _oi >= 0; _oi--) RPO_PUSH((c)->operands[_oi]); \
        if (((c)->op >= IR_MATCH_LIT && (c)->op <= IR_MATCH_ASSIGN_SAVE) || (c)->op == IR_MATCH_DEFER || (c)->op == IR_MATCH_VALUE) RPO_PUSH((c)->ω.node); \
        if ((c)->op == IR_REPALT && (c)->n_operands > 1) RPO_PUSH((c)->operands[1]); \
        if ((c)->op == IR_REPALT && (c)->n_operands > 0) RPO_PUSH((c)->operands[0]); \
        if ((c)->op == IR_MOVE_LABEL && (c)->n_operands > 1) RPO_PUSH((c)->operands[1]); \
        if ((c)->op == IR_MOVE_LABEL && (c)->n_operands > 0) RPO_PUSH((c)->operands[0]); \
        if ((c)->op == IR_CREATE && (c)->n_operands > 0) RPO_PUSH((c)->operands[0]); \
        if ((c)->op == IR_SUSPEND && (c)->n_operands > 1) RPO_PUSH((c)->operands[1]); \
        if (((c)->op == IR_SUBSCRIPT || (c)->op == IR_RANDOM || (c)->op == IR_DEREF || (c)->op == IR_ASSIGN_VAR || (c)->op == IR_REV_ASSIGN_VAR || (c)->op == IR_KEYWORD_ASSIGN || (c)->op == IR_SCAN_TAB || (c)->op == IR_SCAN_MOVE || (c)->op == IR_SCAN_POS || (c)->op == IR_SCAN_MATCH || (c)->op == IR_SCAN_ANY || (c)->op == IR_SWAP_VAR || (c)->op == IR_CALL_VALUE || (c)->op == IR_VAR) && (c)->ω.node) RPO_PUSH((c)->ω.node); \
        if (((c)->op == IR_CALL || ir_is_call_kind((c)->op) || (c)->op == IR_PROC_GEN || (c)->op == IR_ACTIVATE || (c)->op == IR_TO || (c)->op == IR_TO_BY) && (c)->ω.node) RPO_PUSH((c)->ω.node); \
        if ((c)->op == IR_STATEMENT_BEGIN && (c)->ω.node) RPO_PUSH((c)->ω.node); /* fA (this session): STATEMENT_BEGIN.ω is LOAD-BEARING for collection -- DRIVE_PAIR emits `β: jmp ω`, so the wired target must be collected or its label resolves to the main_ω fallback.  MEASURED on wl_1.sno: with fA.γ redirected to the own sbeg, the fJ γ-chains that used to carry fail-target reachability are orphaned; the null `:(LOOP)` statement (reachable ONLY as the GT statement's fallthrough fail target via sbeg.ω) was never collected (6 begins pristine vs 5 armed), and the GT statement's failure exited the program silently (rc=1, zero bytes on both streams, both modes) -- the 175 fossil's mechanism, retired at the collector rather than papered at a consumer.  On pristine shapes every fail target is already γ-reachable so this push is RPO_VISITED-skipped; armed-shape layout may reorder, which is this rung's armed delta. */ \
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
    int _stmt_seed = 0; if (g_is_text && g_emit_cfg && entry_is_own_graph_root) for (int _ss = 0; _ss < g_emit_cfg->n; _ss++) { IR_t * _sn = g_emit_cfg->all[_ss]; if (_sn && _sn->op == IR_STATEMENT_BEGIN) { _stmt_seed = 1; break; } }   /* ⭐⭐⭐ STATEMENT-ORDER (Lon s62 in-chat: "You process each STATEMENT, ONE at a TIME, and in ORDER of the source."): a graph that carries IR_STATEMENT_BEGIN nodes is the SN4-family ONE shared statement stream, and its graph-root emission walks the statements in all[] CREATION ORDER — which is source order, because the lowerer lowers the statement array sequentially into the one graph (no extraction, no lexical body regions, no END_* delimitation; the LBL__ pseudo-proc is just proc_entry_node = the label's anchor IN this same graph).  Each statement_begin seeds its OWN root block (direct queue write, the floater precedent, bypassing RPO_PUSH's begin-refusal conjunct), so DEFINE bodies land BETWEEN the DEFINE statement and its goto target exactly as written, entered only through the shim/registry, jumped over by the statement's own goto.  Emission-order only — every inter-statement edge is an explicit jmp, so behavior is byte-for-byte the same program in source order.  TEXT-gated this seat: the m3 driver still emits LBL__ standalone chains (its registration takes slab fn pointers from that emission), and re-walking the shared graph here would double-emit those nodes — the SAME owed m3 slice as the s58 tiny shim / fold arm; deleting the m3 standalone + intern-record registration + lifting this gate is ONE coherent future rung. */
    if (_stmt_seed) { if (qt < Q_MAX) queue[qt++] = entry; RPO_DRAIN(); RPO_FLUSH(); for (int _ss = 0; _ss < g_emit_cfg->n; _ss++) { IR_t * _sb = g_emit_cfg->all[_ss]; if (!_sb || _sb->op != IR_STATEMENT_BEGIN || RPO_VISITED(_sb)) continue; if (qt < Q_MAX) { queue[qt++] = _sb; RPO_DRAIN(); RPO_FLUSH(); } } }
    else { RPO_PUSH(entry); RPO_DRAIN(); RPO_FLUSH(); }
    { extern int zls_g_group_count(const IR_graph_t *); extern const IR_t * zls_g_group_anchor(const IR_graph_t *, int);
      if (g_emit_cfg && entry_is_own_graph_root && !_stmt_seed) { int _gc = zls_g_group_count(g_emit_cfg); for (int _k = 0; _k < _gc; _k++) { const IR_t * _a = zls_g_group_anchor(g_emit_cfg, _k); if (_a && !emit_chain_entry_already_emitted(_a)) { RPO_PUSH(_a); RPO_DRAIN(); RPO_FLUSH(); } } } }   /* STATEMENT-ORDER (Lon s62): under _stmt_seed the anchors ARE statement begins and every one is already seeded at its source position — the pull-in would only append duplicates-by-marks or out-of-order tails. */   /* SN4-M34-5a: skip an anchor some earlier emit_chain call already emitted standalone (the LBL__ case) -- see the fix comment beside emit_chain_entry_already_emitted's definition. */   /* ⭐ EARN-DIV-1 (this session): THE `!g_is_text ||` DISJUNCT DELETED -- it made the ORPHAN-PROOF pull-in UNCONDITIONAL IN BINARY, and that is the whole m3/m4 `[EARN]` divergence (s48 item 3b, 11 files).  MEASURED, not reasoned: on a minimal DEFINE+LOOP repro, INC's own `proc_flat` walk arrives here with n=7 (exactly its own body) and entry=0x432820 vs g_emit_cfg->entry=0x4327d0 -- NOT EQUAL, because codegen_flat_chain_body chased its own local `entry` through the LBL__ landing relay at line ~2251 (HQ s26) while g->entry is the graph's unchased root.  In TEXT the `entry == g->entry` conjunct is therefore FALSE and the pull-in is correctly skipped (measured: m4 stays at n=7); in BINARY `!g_is_text` short-circuited the `||` and the pull-in ran anyway, walking anchors 1/2/3 -- main's own IR_GOTO label landings (LOOP/DONE), none of them INC's -- and dragging main's ENTIRE statement chain into INC's collection: n=7 -> 43.  Those 36 foreign nodes are then emitted a SECOND time by main's own `pat_flat` walk, which reaches the identical IR_t* objects (SN4-FLAT-PROC's shared-graph design, scrip.c:923).  Silent at runtime -- dense nids make the duplicate labels byte-identical rather than colliding, and INC's own fn pointer never reaches the appended tail -- but it is duplicate JIT'd instructions in the sealed slab and a real 1:1 violation of GOAL-MODE34-IDENTICAL.  ⛔ THE PRIOR TWO SESSIONS RULED THIS SITE OUT ON A FALSE PREMISE and it cost them the root cause: s49 recorded "zls_group_mark_anchor has zero callers, the registry is empty, _gc is 0 for this program."  DIRECTLY MEASURED HERE: _gc == 4.  The registry is populated by a DIFFERENT writer than the one grepped for.  Never re-rule-out this site from a caller grep -- print _gc.  ONE AUTHORITY: entry-identity is now the SOLE gate in both media, which is also what BOTH-MEDIUM MANDATORY requires -- a pull-in rule that fires in one medium and not the other was the violation. */
    /* FLOATERS (Lon 2026-08-13): seed each REFERENCED floater as its own FINAL root block of the graph-root call — one RETURN, one FRETURN, one NRETURN, output once, near main, main at the bottom.
     * Referenced = some node OUTSIDE the floater group targets it through γ or ω (the NRETURN call's own γ=RETURN / ω=FRETURN wiring is group-internal and must not count, or every program would grow
     * all three blocks for any one landing).  Direct queue write bypasses RPO_PUSH's floater conjunct; RPO_DRAIN's visited mark still dedupes.  Seed order 1..3 fixes block order RETURN, FRETURN, NRETURN. */
    { if (g_emit_cfg && entry_is_own_graph_root) { int _refd[4] = {0, 0, 0, 0};
          for (int _fi = 0; _fi < g_emit_cfg->n; _fi++) { IR_t * _fn = g_emit_cfg->all[_fi]; int _fk = emit_floater_kind(_fn); if (!_fk || _refd[_fk]) continue;
              for (int _rj = 0; _rj < g_emit_cfg->n && !_refd[_fk]; _rj++) { IR_t * _r = g_emit_cfg->all[_rj]; if (_r && _r != _fn && !emit_floater_member(_r) && (_r->γ.node == _fn || _r->ω.node == _fn)) _refd[_fk] = 1; } }
          if (_refd[3]) _refd[1] = _refd[2] = 1;   /* the SNO$NRET call is wired γ=RETURN / ω=FRETURN, so a referenced NRETURN pulls both siblings in — group-internal wiring, invisible to the outside-only scan above (claws5 ASM_FAIL witness: FRETURN referenced from n1208_call, never defined) */
          for (int _fk = 1; _fk <= 3; _fk++) { if (!_refd[_fk]) continue; for (int _fi = 0; _fi < g_emit_cfg->n; _fi++) { IR_t * _fn = g_emit_cfg->all[_fi]; if (!_fn || emit_floater_kind(_fn) != _fk || RPO_VISITED(_fn)) continue;
              if (qt < Q_MAX) { queue[qt++] = _fn; RPO_DRAIN(); RPO_FLUSH(); } } } } }
    /* pass 2: generator ω tails not reached by pass 1 -- iterate to convergence (the RPO refactor
     * made this a one-shot loop over nodes[0..n-1], missing ω-tails of generators added BY pass-2
     * itself; chained every-loops: n3/IR_TO_BY ω→n5 adds n5..n9 incl n8/IR_TO_BY, whose ω→n10
     * was never checked because the loop already ended at the pass-1 n; BFS anchor pushed generator
     * ω-tails mid-drain and was naturally iterative -- reproduce that by re-scanning until stable) */
    { int _p2_base = 0; do { int _p2_added = 0; for (int i = _p2_base; i < n; i++) if (ir_is_generator_kind(nodes[i]->op) && nodes[i]->ω.node && !RPO_VISITED(nodes[i]->ω.node)) { RPO_PUSH(nodes[i]->ω.node); _p2_added = 1; } _p2_base = n; RPO_DRAIN(); RPO_FLUSH(); if (!_p2_added) break; } while (1); }
#undef RPO_FLUSH
#undef RPO_DRAIN
#undef RPO_PUSH_SUCCS
#undef RPO_PUSH
#undef RPO_TAG_EMIT
#undef RPO_MARK
#undef RPO_VISITED
    if (qt >= Q_MAX) { fprintf(stderr, "[GZ-7] FATAL: chain traversal queue saturated (qt=%d >= Q_MAX=%d) for prefix=%s -- control-flow edges were silently dropped; raise CH_MAX\n", qt, (int)Q_MAX, prefix); abort(); }
    unsigned char *zd_on = (unsigned char *)alloca((size_t)(n > 0 ? n : 1)); int *zd_out = (int *)alloca(sizeof(int) * (size_t)(n > 0 ? n : 1)); int *zd_gp = (int *)alloca(sizeof(int) * (size_t)(n > 0 ? n : 1)); int *zd_wp = (int *)alloca(sizeof(int) * (size_t)(n > 0 ? n : 1));
    if (!g_emit.flat_jmp_entry) g_emit.flat_pat = 0;   /* [ALPHA] s32 STALE-FLAT-PAT FIX: emit_jmp_entry_for_patproc sets flat_pat=1 for PAT$ blobs; emit_jmp_entry_clear resets it, but only jmp-entry chains call clear before their emit_chain. A main chain that follows a PAT$ blob emission inherits flat_pat=1, making zd_plan's FENCE1 zdyn veto at emit.cpp:1975 believe it is inside a blob and fire unnecessarily -- declining match-construction statements containing FENCE1 that are provably outside any blob. Guard: jmp-entry chains legitimately have flat_pat=1 (set by their arm function before emit_chain) and must keep it. Non-jmp-entry chains have no mechanism to set flat_pat=1 before their own emit_chain, so any 1 they see is stale from a previous blob. MEASURED: 10 spurious FENCE1 vetoes eliminated; 105->99 declined runs; s32. */
    zd_plan(nodes, n, zd_on, zd_out, zd_gp, zd_wp);   /* ZD-1: the ONE execution-order walk runs ONCE per chain, before any emission, over the same nodes[] the loop below lays down */
    zd_depth_census(nodes, n, zd_on, zd_out, zd_gp, zd_wp, prefix);   /* ⭐ ZD-DEPTH (this session): READ-ONLY brick-wall census -- see the function's own header. Inert unless SCRIP_ZD_DEPTH=1; zero g_emit writes, zero emission. NOTE (U-1b finding): the census measures multi-depth JOINS within the armed set -- nodes whose inbound edges arrive at different accumulated depths.  U-1b's retarget removes the WHACK from non-join ω edges (those that escape the statement to :F targets outside nodes[]); those never appeared as joins and so the census reads 469 regardless of SCRIP_UNWIND.  The behavioral acceptance instrument for U-1b is BY SET both modes + uw3 rc=0 (not the census). */   /* ⭐ ZD-DEPTH (this session): READ-ONLY brick-wall census -- see the function's own header. Inert unless SCRIP_ZD_DEPTH=1; zero g_emit writes, zero emission. */
    { /* ZK-0 (s212): per-graph cell census. READ-ONLY: zero g_emit writes, zero emission. Inert unless SCRIP_ZD_CENSUS=1. Accumulates graph-level: total K*bytes of armed nodes, armed node count, declined run first-blocker kind histogram, .bss arena bytes. FALSIFIABILITY: (1) a known-armed run (IR_TO with K=32) moves census_k_total up by 32; (2) a known-declined graph (locals-only, pin=0) shows blocker=IR_VAR in the histogram -- these two injections are the ZK-0 completion criterion before any rung code lands. icn_cells_graph=1 flag (set by lower_icon.c when SCRIP_ICN_CELLS=1) routes this printout; flag=0 graphs are SN4/Prolog and their line is suppressed so the census is not polluted. */
    static int _zk0_on = -1;
    if (_zk0_on < 0) { const char * _e = getenv("SCRIP_ZD_CENSUS"); _zk0_on = (_e && *_e == '1') ? 1 : 0; }
    if (_zk0_on && g_emit_cfg && g_emit_cfg->icn_cells_graph) {
        long _k_total = 0; int _armed = 0; int _declined_runs = 0;
        /* blocker histogram: count first-blocker op per declined run (simple: per undeclined node in a run that was not armed) */
        int _blocker_op[512]; int _blocker_cnt[512]; int _n_blockers = 0;
        /* scan armed nodes */
        int _globals_on_stack = 0;   /* ZK-5 ASSERTION COUNTER: IR_VAR/IR_ASSIGN globals that erroneously landed on the ZD spine -- must stay 0; zd_wl_kind's !_icn_cells conjunct is the ONE guard */
        { extern int is_global(const char *); for (int _i = 0; _i < n; _i++) { if (zd_on[_i]) { _k_total += zd_k(nodes[_i]); _armed++; int _op5 = (int)nodes[_i]->op; if ((_op5 == IR_VAR || _op5 == IR_ASSIGN) && IR_LIT(nodes[_i]).sval && is_global(IR_LIT(nodes[_i]).sval) && !graph_has_local(g_emit_cfg, IR_LIT(nodes[_i]).sval)) _globals_on_stack++; } } }
        /* scan declined runs: a run that starts at a non-armed node whose zd_wl_kind returns 1 but zd_plan declined it -- first non-admitted node is the blocker */
        for (int _i = 0; _i < n; _i++) {
            if (zd_on[_i]) continue;
            if (!zd_wl_kind(nodes[_i])) {
                /* this node itself is not admitted -- is the blocker of its own run */
                int _op = (int)nodes[_i]->op; int _found = 0;
                for (int _b = 0; _b < _n_blockers; _b++) if (_blocker_op[_b] == _op) { _blocker_cnt[_b]++; _found = 1; break; }
                if (!_found && _n_blockers < 512) { _blocker_op[_n_blockers] = _op; _blocker_cnt[_n_blockers++] = 1; }
                _declined_runs++;
            }
        }
        /* .bss arena bytes = jcon_value_region (the WHOLE-GRAPH slot reservation, the residual bss-backed pre-carve) */
        int _bss_bytes = g_emit_cfg->jcon_value_region;
        fprintf(stderr, "[ZK-CENSUS] graph=%s icn_cells=1 n=%d armed=%d K_total=%ld declined_nodes=%d bss_arena=%d globals_on_stack=%d\n",
                prefix ? prefix : "?", n, _armed, _k_total, _declined_runs, _bss_bytes, _globals_on_stack);   /* ZK-5: globals_on_stack must be 0 -- asserted by test_gate_icn_zk5_gva.sh */
        for (int _b = 0; _b < _n_blockers && _b < 8; _b++) {
            const char * _nm = bb_op_name((IR_e)_blocker_op[_b]);
            fprintf(stderr, "[ZK-CENSUS-BLOCKER]   %-30s count=%d\n", _nm ? _nm : "<unknown>", _blocker_cnt[_b]);
        }
    } }   /* ZK-0 census END: pure accumulate+print, zero side effects. */
    { /* PL-ZK-0 (s3): per-graph Prolog cell census. READ-ONLY: zero g_emit writes, zero emission. Inert unless SCRIP_ZD_CENSUS=1. Same structure as the Icon census above; gated on pl_cells_graph so Icon/SN4 output is not polluted. FALSIFIABILITY: (1) a known-armed run moves armed UP; (2) a known-declined graph names the first blocker. Both injections required before rung declared complete. */
    static int _plzk0_on = -1;
    if (_plzk0_on < 0) { const char * _e = getenv("SCRIP_ZD_CENSUS"); _plzk0_on = (_e && *_e == '1') ? 1 : 0; }
    if (_plzk0_on && g_emit_cfg && g_emit_cfg->pl_cells_graph) {
        long _k_total = 0; int _armed = 0; int _declined_nodes = 0;
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
                _declined_nodes++;
            }
        }
        int _bss_bytes = g_emit_cfg->jcon_value_region;
        fprintf(stderr, "[PL-ZK-CENSUS] graph=%s pl_cells=1 n=%d armed=%d K_total=%ld declined_nodes=%d bss_arena=%d\n",
                prefix ? prefix : "?", n, _armed, _k_total, _declined_nodes, _bss_bytes);
        for (int _b = 0; _b < _n_blockers && _b < 8; _b++) {
            const char * _nm = bb_op_name((IR_e)_blocker_op[_b]);
            fprintf(stderr, "[PL-ZK-CENSUS-BLOCKER]   %-30s count=%d\n", _nm ? _nm : "<unknown>", _blocker_cnt[_b]);
        }
    } }   /* PL-ZK-0 census END */
    /* ZW-5 O-2: per-depth ω stub pool.  Pre-compute after zd_plan; emit stubs after each IR_STATEMENT's drive. */
    enum { ZW5_POOL = 128, ZW5_MAX_DEPTHS = 8 };
    bb_label_t zw5_pool[ZW5_POOL]; int zw5_base = 0;
    int zw5_stmt_idx[ZW5_POOL]; int zw5_stmt_depths[ZW5_POOL][ZW5_MAX_DEPTHS]; int zw5_stmt_cnt[ZW5_POOL]; int zw5_pool_stmts = 0;
    static int _zw5_on = -1; if (_zw5_on < 0) { const char *_e = getenv("SCRIP_ZW5"); _zw5_on = (_e && *_e == '1') ? 1 : 0; }   /* DEFAULT OFF (Lon 2026-08-02, observer seat): per-depth ω stub ladder DEPRECATED -- 100 stubs/roman, and its wpop-steal wedged 067 post-output (rsp mispositioned into NV_SET, stdio wedge; SCRIP_ZW5=0 cures, ZD_MATCH exonerated). The ruled design is the STF ___ bracket (ZGPOP-STF law above: mov rsp,___ = depth-independent cut, ONE release site per statement) extended to the armed-pattern population this ladder covered -- see FINDING-2026-08-02h. */
    if (_zw5_on) { for (int _i = 0; _i < n; _i++) { if (nodes[_i]->op != IR_STATEMENT || !zd_on[_i]) continue; int _stno = (int)IR_LIT(nodes[_i]).ival; int _depths[ZW5_MAX_DEPTHS]; int _ndepths = 0; for (int _j = _i - 1; _j >= 0; _j--) { if (zd_on[_j] && zd_wp[_j] > 0) { int _w = zd_wp[_j]; int _dup = 0; for (int _d = 0; _d < _ndepths; _d++) if (_depths[_d] == _w) { _dup = 1; break; } if (!_dup && _ndepths < ZW5_MAX_DEPTHS) _depths[_ndepths++] = _w; } if (nodes[_j]->op == IR_STATEMENT) break; }
        if (_ndepths == 0 || zw5_base + _ndepths > ZW5_POOL) continue;
        int _slot = zw5_pool_stmts++; zw5_stmt_idx[_slot] = _i; zw5_stmt_cnt[_slot] = _ndepths;
        for (int _d = 0; _d < _ndepths; _d++) { zw5_stmt_depths[_slot][_d] = _depths[_d]; emit_label_initf(&zw5_pool[zw5_base + _d], "%s_zw5s%d_ω_d%d", prefix, _stno, _depths[_d]); }
        zw5_base += _ndepths; } }
    { g_emit.flat_all_zd = 0; { static int _zpg = -1; if (_zpg < 0) { const char *_e = getenv("SCRIP_ZD_PL_GEN"); _zpg = (_e && *_e == '0') ? 0 : 1; } int _pl_cells = (_zpg && g_emit_cfg && g_emit_cfg->pl_cells_graph); int _gen_ok = (!g_emit.flat_gen) || _pl_cells; int _jmp_ok = (!g_emit.flat_jmp_entry) || _pl_cells; /* PL-ZK-5: pl_cells_graph lifts BOTH flat_gen AND flat_jmp_entry exclusions.  (1) flat_gen: Prolog generator graphs eligible for flat_all_zd=1 on cells arm -- each retry re-enters at proc α (full prologue, fresh cells carve); cells are per-invocation, not retained across retries.  (2) flat_jmp_entry: Prolog retry goes to α via g_pl_retry pop (bb_indirect_goto ZD arm), never to lbl_res (Icon coexpr re-entry point); lbl_res protocol is irrelevant on the Prolog cells arm.  SN4/Icon watermarks: pl_cells_graph and icn_cells_graph mutually exclusive (R-ZK-A); SN4 never sets pl_cells_graph; byte-identical for both.  Killswitch SCRIP_ZD_PL_GEN=0 restores both exclusions atomically.  ONE AUTHORITY: this is the only site that gates flat_all_zd on flat_gen/flat_jmp_entry. */ if (!g_emit.flat_pat && _gen_ok && _jmp_ok && x86_zc_frame() == ZC_FRAME_RSP && n > 0) { int _azd = 1; for (int _i = 0; _i < n; _i++) if (!zd_on[_i]) { if (_pl_cells && zd_out[_i] == -1) continue;   /* PL-ZK-5B BUG1 FIX: zd_out==-1 is zd_plan's init sentinel meaning "never visited by any run" -- structurally absent β-continuation nodes in multi-clause proc_flat graphs (e.g. n13/n26 in fib: unreachable retry DISJUNCTION clause bodies). These are NOT declined nodes; zd_plan simply never walked them. Vetoing flat_all_zd on them is wrong: the armed run (13 nodes) is self-consistent; the 47 absent nodes never execute on the primary gamma-chain. Additive guard: only skips truly-never-visited nodes (zd_out==-1), not nodes that were walked and declined (zd_out>=0, zd_on=0). SN4/Icon: every node IS reachable in their graphs, so zd_out is never -1 at this point -- byte-identical by construction. ONE AUTHORITY: this is the only LP-2 gate site. */ _azd = 0; break; } g_emit.flat_all_zd = _azd; } } { static int _lpd = -1; if (_lpd < 0) { const char * _e = getenv("SCRIP_LP_DIAG"); _lpd = (_e && *_e == '1') ? 1 : 0; } if (_lpd) { int _arm = 0; for (int _i = 0; _i < n; _i++) if (zd_on[_i]) _arm++; fprintf(stderr, "[LP] prefix=%s n=%d armed=%d all_zd=%d region=%d jmp=%d pat=%d gen=%d\n", prefix, n, _arm, g_emit.flat_all_zd, (g_emit_cfg ? g_emit_cfg->jcon_value_region : -1), g_emit.flat_jmp_entry, g_emit.flat_pat, g_emit.flat_gen); } } }   /* LP-2 LAYOUT PASS (s22d NEXT(1)): the verdict is now the ZD PLAN ITSELF, not a kind approximation.  The RPO walk + zd_plan moved ABOVE the prologue (both are PURE -- verified: zero emissions, zero g_emit writes in either), so `flat_all_zd` reads the REAL per-node arm results zd_on[] that the drive loop will use.  ONE AUTHORITY: the predicate and the emission now cannot disagree, because they are the same array.  LP-1 asked "do these kinds LOOK armable?" and could answer yes where zd_plan then declined a statement for a STRUCTURAL reason (cross-statement operand, staging failure) -- leaving legacy FR/FRQ readers live against a carve LP-1 had already zeroed.  This asks "did every node ACTUALLY arm?", so flat_all_zd=1 now means the carve is zero BY CONSTRUCTION.  zd_plan self-declines jmp-entry/non-FORTH (leaving zon all-zero, hence all_zd=0); the flat_pat/flat_gen/RSP guards are kept explicit so the flag means what it says at its source.  Widens automatically as kinds arm -- when IR_CALL lands (ZD-7) call-bearing graphs become carve-free with no edit here, which is the DL-ONESHOT ___-free classifier seed/test_sno_dl_2_no___.s asks for. */    { extern void emit_fb_divergence_check(void); emit_fb_divergence_check(); }   /* ZETA-FB-1 (s160): the LAST point at which every input to BOTH frame-base predicates is final for this graph and the prologue has not yet run — x86_fb_pinned()=emit_jmp_pin____() picks the base every DATA ref NAMES, emit_rec_pin() picks the base the RECORD protocol names, and the prologue dispatched on the next line is what ESTABLISHES one.  A disagreement here is the s158 land mine latent: store and load naming different registers.  Reports under SCRIP_FB_DIVERGE=1, costs one compare otherwise. */
    if (text_externalise && g_is_text) emit_label_define_bb(&lbl_α);
    else if (g_emit.zframe_graph || (g_emit_cfg && g_emit_cfg->icn_cells_graph && g_emit.flat_lcl_proc)) emit_label_define_bb(&lbl_α);   /* ICN-FR-2: zframe dc stub uses &lbl_α as its backward jump target (proc_f_α not body); define it in binary mode too so bb_label_defined(&lbl_α) is true when the X-record patches. Harmless for non-dc zframe graphs (no X-record references lbl_α).  ZK-4 SLICE 2: cells-arm CLASS ZF (icn_cells_graph && flat_lcl_proc) extended: DC stub now jumps to lbl_α so the flat_lcl_proc prologue runs; binary-mode define required for the same reason. Predicate mirrors xa_flat_class_zf() ONE AUTHORITY. */
    /* CARVE-KILL (Lon directive, s22n): XA_FLAT_PROLOGUE dispatch DELETED with its function. No graph emits a prologue. */
    if (g_is_text) g_emit_pos += 7;
    bb_label_t lbl_stcγ, lbl_stcω;   /* STMT-FRAME (s21x-c): the two chain-exit LEAVE cuts -- every statement-crossing edge into lbl_γ/lbl_ω funnels through these (mov rsp,___; pop ___; jmp), the seed's stmt_MAIN_1_end/_fail shape at chain scope; bodies emitted just before the lbl_γ define */
    if (g_emit.flat_stmt_frame) { emit_label_initf(&lbl_stcγ, "%s_stγ", prefix); emit_label_initf(&lbl_stcω, "%s_stω", prefix); }
    if (g_emit.zframe_graph) {   /* ICN-FR-2: ζ-FRAME PROLOGUE (keeper) */
        { extern void xa_flat_zframe_prologue(void); xa_flat_zframe_prologue(); }
        if (g_emit_cfg && g_emit_cfg->pl_cells_graph && g_emit.flat_all_zd) { int _plk = 0; for (int _i = 0; _i < g_emit_cfg->n; _i++) { IR_t * _nd = g_emit_cfg->all[_i]; if (_nd && zd_wl_kind(_nd)) _plk += zd_k(_nd); } if (_plk > 0) { bb_emit_x86(x86("sub", "rsp", (long)_plk)); } }   /* PL-ZK-3 COMPLETE (s6): PROC-ENTRY CELLS CARVE gated on flat_all_zd (Option A, Lon ruling).  flat_all_zd=1 iff EVERY node armed by zd_plan -- no UCLAIM path possible by construction (LP-2 sets flat_all_zd from the SAME zd_on[] array the drive loop uses; ONE AUTHORITY, cannot disagree).  When flat_all_zd=0 (any declined node -- UCLAIM fires), the carve is suppressed and UCLAIM path runs byte-identically to cells=0 -- RSP_stmt_entry = ___ - flat_frame_bytes as zvo_resolve expects.  Gate makes the two regimes mutually exclusive: flat_all_zd=1 => carve fires, UCLAIM cannot fire (no declined nodes); flat_all_zd=0 => carve suppressed, UCLAIM path correct.  TIMING: flat_all_zd set by LP-2 before prologue dispatch; this carve runs inside the prologue arm -- correct ordering, no circularity.  The zframe prologue (xa_flat_zframe_prologue) carves flat_frame_bytes for the ZLS frame and pins ___=rsp.  When pl_cells_graph=1 AND flat_all_zd=1 the planner additionally stages K_total=sum_zd_k(admitted) bytes of per-BB cells on the FORTH spine BELOW the ZLS frame.  FIX: sum zd_k over all admitted nodes -- THE ONE AUTHORITY (zd_k, same function the planner calls; this scan calls it directly to avoid a second spelling) -- and emit sub rsp,_plk immediately after the zframe prologue while ___ is pinned.  CORRECTNESS: _plk is the STATIC maximum cells depth (sum over all arms admitted, non-popping -- they stack); the epilogue lea rsp,[___+flat_frame_bytes] unwinds from ANY in-body rsp depth (depth-immune by ___ pin).  GATED pl_cells_graph: SN4 never sets pl_cells_graph; Icon uses icn_cells_graph on a mutually exclusive path (R-ZK-A); byte-identical for both by construction.  KILLSWITCH: SCRIP_PL_CELLS=0 leaves pl_cells_graph=0.  BOTH-MEDIUM: bb_emit_x86(x86("sub",...)) routes through the x86() encoder.  ONE AUTHORITY: this site is the only proc-entry cells carve; zd_plan K_total and this scan agree by construction (both call zd_k).  s22k law: _plk is computed by the ONE K function, never re-spelled. */
    } else if (g_emit.flat_lcl_proc) {   /* ICN-PROC-FRAME (s211): jmp-entry lexical proc with params/locals.  Prologue: sub rsp by kt+(np+nl)*16 to carve wire-header+param+local slots; save γ/ω wires and caller-___ in the header; pin ___=rsp; call rt_lcl_proc_args_install(___,np,nl) to copy g_call_args into [___-(i+1)*16] and zero locals.  BOTH MEDIUM: TEXT and BINARY both emit a C call so no medium-specific address arithmetic is needed.  Params address: [___-16] .. [___-np*16]; locals: [___-(np+1)*16] .. [___-(np+nl)*16].  Wire-header layout (same as BLOB-GRANT): [kt-24]=γ [kt-16]=ω [kt-8]=caller-___; at entry rsp==___, so [rsp+kt-24]==[___+kt-24]. */
        int kt2 = g_emit.flat_frame_bytes;
        int np = g_emit_cfg ? g_emit_cfg->nparams : 0;
        int nl = g_emit_cfg ? g_emit_cfg->nlocals : 0;
        int frame_total = kt2 + (np + nl) * 16;
        extern void rt_lcl_proc_args_install(void *, int, int);
        extern void rt_icn_zframe_args_install(void *, int, int);   /* ZK-4 SLICE 2: cells-arm DC path uses this; reads g_call_args[] directly, no g_pcall dependency */
        int _use_zframe_install = (g_emit_cfg && g_emit_cfg->icn_cells_graph) ? 1 : 0;   /* ZK-4 SLICE 2 ONE AUTHORITY: cells-arm flat_lcl_proc procs entered via DC stub have g_pcall_top=0 (no pcall record pushed), so rt_lcl_proc_args_install reads nargs=0 and installs nothing; rt_icn_zframe_args_install reads g_call_args[] which rt_arg_stage populated. Predicate mirrors xa_flat_class_zf() + the DC stub condition. SNOBOL4/Prolog/Raku: icn_cells_graph=0, byte-identical. */
        if (g_is_text) {
            char _lp[512]; int _lz = 0;
            _lz += snprintf(_lp + _lz, (int)sizeof(_lp) - _lz, "sub rsp, %d\nmov qword ptr [rsp + %d], rcx\nmov qword ptr [rsp + %d], rdx\n", frame_total, frame_total - 24, frame_total - 16, frame_total - 8);
            _lz += snprintf(_lp + _lz, (int)sizeof(_lp) - _lz, "mov rdi, rsp\nmov esi, %d\nmov edx, %d\ncall %s@PLT\n", np, nl, _use_zframe_install ? "rt_icn_zframe_args_install" : "rt_lcl_proc_args_install");
            emit_text_n(_lp, strlen(_lp));
        } else {
            if (frame_total <= 127) { ef_b3(0x48, 0x83, 0xEC); ef_b1((uint8_t)frame_total); } else { ef_b3(0x48, 0x81, 0xEC); bb_emit_u32((uint32_t)frame_total); }
            { int _d = frame_total - 24; ef_b2(0x48, 0x89); if (_d >= -128 && _d <= 127) { ef_b3(0x4C, 0x24, (uint8_t)(int8_t)_d); } else { ef_b2(0x8C, 0x24); bb_emit_u32((uint32_t)_d); } }
            { int _d = frame_total - 16; ef_b2(0x48, 0x89); if (_d >= -128 && _d <= 127) { ef_b3(0x54, 0x24, (uint8_t)(int8_t)_d); } else { ef_b2(0x94, 0x24); bb_emit_u32((uint32_t)_d); } }
            { int _d = frame_total - 8;  ef_b2(0x48, 0x89); if (_d >= -128 && _d <= 127) { ef_b3(0x6C, 0x24, (uint8_t)(int8_t)_d); } else { ef_b2(0xAC, 0x24); bb_emit_u32((uint32_t)_d); } }
            ef_b3(0x48, 0x89, 0xE7);   /* mov rdi,rsp */
            ef_b1(0xBE); bb_emit_u32((uint32_t)np);   /* mov esi,np (imm32) */
            ef_b1(0xBA); bb_emit_u32((uint32_t)nl);   /* mov edx,nl (imm32) */
            { uint64_t _fn = (uint64_t)(uintptr_t)(void *)(_use_zframe_install ? rt_icn_zframe_args_install : rt_lcl_proc_args_install); ef_b2(0x48, 0xB8); bb_emit_u64(_fn); ef_b2(0xFF, 0xD0); }   /* movabs rax,fn; call rax */
        }
    }
    if (!bare) emit_label_define_bb(&lbl_α_body);   /* BARE-CHAIN: no _α_body alias — nothing references it for SN4 flat proc chains (verified: zero refs in roman.s), and a stray one would fail loudly at assembly, which is the tripwire we want */
    { extern int g_flat_outer_nparams; static int _gsym = -1; if (_gsym < 0) { const char * e = getenv("SCRIP_GLUE_SYM"); _gsym = (e && *e == '1') ? 1 : 0; } int _legacy = (!g_emit.flat_jmp_entry && !g_emit.flat_pat && !g_emit.flat_gen && !g_gen_proc_active && !g_emit.zframe_graph && !g_emit.flat_lcl_proc); /* ICN-FR-2: zframe_graph excluded — ζ-frame graphs establish their own ___ pin in the ζ-frame prologue; GLUE-O must not emit a second push/mov that overwrites ___ and corrupts [___+kt-24/-16/-8] wire reads.  ZK-2 (s226): flat_lcl_proc excluded by the same law — the flat_lcl_proc prologue (sub rsp,kt + wire-header save + mov ___,rsp) already pins ___; GLUE-O's push ___ at lbl_α_body would overwrite that pin and clobber the wire-header at [___+kt-24/-16/-8].  Cells-arm outer main is the new customer (flat_jmp_entry=0, flat_lcl_proc=1 after s226's line-3000 fix). */ extern int g_glue_entered; g_glue_entered = (g_emit.flat_outer_nparams == 0 && _legacy) ? 1 : 0; if (g_glue_entered) bb_emit_x86(x86_main_prologue()); (void)_gsym; { static int _gluo = -1; if (_gluo < 0) { const char * e = getenv("SCRIP_GLUEO"); _gluo = (e && *e == '0') ? 0 : 1; } static int _gluod = -1; if (_gluod < 0) { const char * e = getenv("SCRIP_GLUEO_DIAG"); _gluod = (e && *e == '1') ? 1 : 0; } extern int g_glue_o_sup; g_glue_o_sup = (_gluo && g_glue_entered && !emit_rec_pin()) ? 1 : 0; if (g_glue_o_sup) g_glue_entered = 0; if (_gluod) fprintf(stderr, "[GLUEO] graph=%s entered=%d rec_pin=%d deep=%d pat=%d gen=%d -> closed_loop_suppressed=%d\n", g_emit.flat_lbl_α ? g_emit.flat_lbl_α : "<anon>", g_glue_entered, emit_rec_pin() ? 1 : 0, g_emit.flat_deep_arrival, g_emit.flat_pat, g_emit.flat_gen, g_glue_o_sup); } if (g_glue_entered) { { long _capN = 0; if (g_emit_cfg) for (int _ci = 0; _ci < g_emit_cfg->n; _ci++) { IR_t * _cs = g_emit_cfg->all[_ci]; if (_cs && _cs->op == IR_MATCH_ASSIGN_SAVE && cap_anchor_of(_cs) > 0) _capN++; } g_emit.op_fc_bytes = _capN > 0 ? (long)(64 + 16 * _capN) : 0; }   /* ⭐ OS-2·SLICE-1 CAPTURE REGION: the GLUE-O enter is the ONE carve-less pin -- widen its K to back the anchored-capture δ slots with REAL memory ([___-64-16k], k=1..N; 64 clears the stfh HKQ zone; K multiple of 16 + the glue pad-8 preserves entry parity exactly).  N computed through THE ONE ROUTING AUTHORITY so staging and spelling cannot disagree; N==0 (incl. SCRIP_OS_CAP=0) leaves K=0 -- byte-identical legacy per the killswitch law.  framed_leave's whack (mov rsp,___) frees the region at any depth on every exit; FENCE1/ABORT graphs are declined AT THE AUTHORITY because their mid-graph floor whacks would free live slots. */ bb_emit_x86(bb_glue_framed_enter()); } }   /* ⭐⭐ GLUE-O (O-6 / ZW-6 CLASS O, s26a+s26b) -- THE OUTER FRAME IS CLOSED-LOOP CEREMONY FOR THE DEPTH-STATIC MAJORITY.  212/318 programs establish ___, restore it, and never read it; all 634 CLASS O mov-rsp-___ sites at main_γ/ω are immediately followed by call exit@PLT.  Predicate !emit_rec_pin() is precisely "no data reader of this frame can exist".  DEFAULT ON (s26b); opt-out SCRIP_GLUEO=0.  Census at s26b: bearing 317->132, push____ 464->252 (-212), data_refs 7893->7893 (law-4 pop untouched).  GATES proven on all three language watermarks (SNOBOL4/Icon/Prolog) before flip. */   /* ⭐⭐⭐ GLUE-SYM (s206): THE ENTER AND THE WHACK ARE ONE AUTHORITY AND THEY HAD DRIFTED APART.  bb_glue_outer_γ/ω both emit IF(bb_glue_outer_whack(), bb_glue_framed_leave()) and bb_glue_outer_whack() returns true UNCONDITIONALLY, so EVERY outermost graph executes `mov rsp,___; pop ___` -- while this site skipped the matching `push ___; mov ___,rsp` for the pat/gen/jmp-entry/gen-proc classes.  bb_glue_framed.cpp's OWN header names the resulting shape as the failure mode in so many words: "Reversing (1)/(3) or omitting (1) while keeping (3) loads the CRT caller's ___ into rsp."  MEASURED on a 7-line repro (procedure g() with two suspends, every write(g())): proc_g's region carries push ___=0, mov rsp,___=2, pop ___=3 -- a whack against a base nobody established plus a 24-byte pop imbalance, and the program SEGVs.  The same shape is why 6,264 of Icon's 37,872 [___+-N] data refs sit in regions with no seed (census: scripts/test_gate_icn_____census_ratchet.sh).  This did not regress: CARVE-KILL (ef9a7d2c/1ba33ea6, Lon "delete the prolog and epilog ... a nice broken system to build from") deleted xa_flat_prologue, which is what USED to seed ___ for exactly these classes, and the enter that replaced it inherited a guard written when the prologue still covered the remainder.  UNDER SCRIP_GLUE_SYM=1 the enter fires wherever the whack will, restoring the invariant; the nparams==0 conjunct is KEPT because a parameterised entry marshals args under a different protocol and is a separate rung.  OPT-IN FIRST per the s203 ZW-1 lesson (an opt-OUT flip of a SHARED default is what cost Icon 30 programs while the ledger recorded a SNOBOL4 accounting) -- this file serves SNOBOL4 and Prolog too, so the default flip needs all three watermarks re-proven.  NAMED FOR WHAT IT DECIDES, never for a language, per the NO-LANGUAGE-IDENTITY FACT RULE. */   /* ONE-SHOT BRIDGE α-side (Lon s22p, atomic with bb_glue_outer_whack=true): pin ___=rsp for the outermost main graph ONLY -- the whack at γ/ω restores rsp to this base regardless of how deep the non-popping spine wandered.  Guard mirrors the STMT_FRAME outer-regime conjuncts: jmp-entry / pat-blob / gen / gen-proc keep their own protocols verbatim.  op_fc_bytes=0 because this is the BRACKET glue (K=0 pad, same as STMT_FRAME's head stub) not a value cell; the actual K for any per-BB cells comes from the node's own zw_node_k via op_fc_bytes at the drive choke, which runs later. */
    bb_label_t **lbls  = (bb_label_t **)alloca(sizeof(bb_label_t *) * n);
    bb_label_t **betas = (bb_label_t **)alloca(sizeof(bb_label_t *) * n);
    bb_label_t **ra_y  = (bb_label_t **)alloca(sizeof(bb_label_t *) * n);
    bb_label_t **ra_t  = (bb_label_t **)alloca(sizeof(bb_label_t *) * n);
    bb_label_t **na_s  = (bb_label_t **)alloca(sizeof(bb_label_t *) * n);
    bb_label_t ***fc_sig = (bb_label_t ***)alloca(sizeof(bb_label_t **) * n);
    bb_label_t **na_f  = (bb_label_t **)alloca(sizeof(bb_label_t *) * n);
    bb_label_t **st_pre = (bb_label_t **)alloca(sizeof(bb_label_t *) * n);   /* STMT-FRAME (s21x-c): per-statement-head enter stub label -- minted for every bb_src_of head when the regime is live, NULL otherwise; every cross-edge into a head lbls[k] retargets here (slice-1 whitelist has no gotos, so EVERY edge into a head is a genuine statement entry -- no membership map needed) */
    int st_first_seen = 0;
    bb_label_t **st_x   = (bb_label_t **)alloca(sizeof(bb_label_t *) * n);   /* STMT-FRAME: the head's LEAVE half -- a statement-crossing edge must cut the SOURCE bracket before the dest's enter (the seed's stmt_*_end/_fail law: leave THEN jmp), so st_x[k] emits bb_glue_framed_leave (GLUE-4) and FALLS THROUGH into st_pre[k].  NULL for the first-emitted head: its only entrant is the prologue fallthrough, which has no bracket to cut (and cutting there would load the CRT caller's ___ into rsp -- the 058 disease). */
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
    { extern int zls_off(const IR_t *); for (int _oi = 0; _oi < n; _oi++) if (nodes[_oi]->op == IR_MATCH_BEGIN) { int _ho = zls_off(nodes[_oi]); if (_ho >= 0) own_mark = _ho + 16; break; } }
    for (int i = 0; i < n; i++) {
        int _uid = g_flat_node_id++; const char * _kn = flat_label_kind(nodes[i]->op);
        lbls[i]  = emit_label_alloc("n%d_%s_α", _uid, _kn);
        { int _flk = emit_floater_kind(nodes[i]); if (_flk) lbls[i] = emit_floater_label(_flk); }   /* FLOATERS (Lon 2026-08-13): the singleton landing's α IS the bare program-wide label (RETURN / FRETURN / NRETURN) — the ONE shared bb_label_t, defined here in the graph-root chain, referenced by name from every jumper in every chain.  A node-numbered label is a chain resident, not a floater. */
        if (g_emit_cfg && g_emit_cfg->n_balias > 0) { for (int _bi = 0; _bi < g_emit_cfg->n_balias; _bi++) if (g_emit_cfg->balias_node[_bi] == nodes[i]) { lbls[i] = emit_label_alloc("%s", g_emit_cfg->balias_name[_bi]); break; } }   /* BODY-ALIAS (Lon s62): a DEFINE entry statement's α IS <FN>_body — bound at the statement, at its source position, one label; every reference (prior statement's fall-through jmp, shim fold transfer, computed-goto fold, LBL__ registration) resolves to it by construction */
        betas[i] = emit_label_alloc("n%d_%s_β", _uid, _kn);
        ra_y[i]  = (nodes[i]->op == IR_REPALT) ? emit_label_alloc("n%d_%s_ry", _uid, _kn) : NULL;
        ra_t[i]  = (nodes[i]->op == IR_REPALT) ? emit_label_alloc("n%d_%s_rt", _uid, _kn) : NULL;
        na_s[i]  = ((nodes[i]->op == IR_MATCH_ALTERNATE || nodes[i]->op == IR_MATCH_ARBNO || nodes[i]->op == IR_MATCH_FENCE1 || nodes[i]->op == IR_SCAN_SEQUENCE || nodes[i]->op == IR_SCAN_ALTERNATE || nodes[i]->op == IR_DISJUNCTION) && nodes[i]->n_operands > 0) ? emit_label_alloc("n%d_%s_as", _uid, _kn) : NULL;   /* SN4-NARY-ALT/-ARBNO success-glue */
        na_f[i]  = (((nodes[i]->op == IR_MATCH_ALTERNATE || nodes[i]->op == IR_MATCH_ARBNO || nodes[i]->op == IR_MATCH_FENCE1 || nodes[i]->op == IR_SCAN_SEQUENCE || nodes[i]->op == IR_SCAN_ALTERNATE || nodes[i]->op == IR_DISJUNCTION) && nodes[i]->n_operands > 0) || nodes[i]->op == IR_MATCH_BEGIN) ? emit_label_alloc("n%d_%s_af", _uid, _kn) : NULL;   /* SEAL-UNWIND: BEGIN gets a phi/na_f landing = its own anchor-exhaust unwind entry (bb_match_begin defs it at L(1)); pat_seal kills route here instead of bypassing the omega-choke */   /* SN4-NARY-ALT/-ARBNO fail-glue */
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
    /* ⭐⭐⭐ U-1b UNWIND β LIVENESS (Lon HQ directive 2026-08-03c) -- THE CIRCULAR PROPHECY THIS BREAKS: BP-9's dead-β elision (op_beta_dead, staged from bused[] below) removes a box's β trampoline when the
     * WIRE SCAN proves no edge carries the β port tag at it.  That scan mirrors TODAY's consumers, and today nothing rolls into a value-spine β -- so every value-spine β is elided, which is precisely why
     * U-1a's retarget had nowhere to land and measured empty.  Arming the roll without this pass would be WORSE THAN INERT: node_ω = betas[pred] patches a rel32 at a label that was never defined (TEXT =
     * assembler error, BINARY = wild jump).  So the liveness must be computed BEFORE emission, from the graph alone.  DELIBERATELY CONSERVATIVE, and the asymmetry is the whole design: over-marking costs a
     * 2-instruction dead trampoline (bytes only, gate-on only), under-marking is a wild jump -- so this evaluates only the conjuncts that are pure graph properties (armed · statement-terminal wp · widened
     * kind · in-statement bracket · NODE-EXACT chased-ω agreement with a LATER member) and deliberately does NOT model the drive loop's runtime conjuncts (op_wpop / omega_is_beta / omega_is_phi / scan_live
     * / flat_stmt_frame), each of which can only SUPPRESS a retarget.  The marked set is therefore a strict superset of the rolled-into set, which is the safe direction.  Gate-on only: SCRIP_UNWIND unset
     * leaves bused[] byte-identical, so the whole rung stays behind the one killswitch. */
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
                resume_init_lbl->name, emit_rec_rsp_arm() ? "rsp" : emit_rec_fb(), g_suspend_resume_slot);   /* REG-7 U5: the resume slot is a FRAME slot — fb, not zr (zr sealed to rsp; the old flat_pat zr="___" coincidence is gone).  DEL-T1 D-2: the orphaned blob class stores off rsp — this site is the chain head, where rsp IS the flat base (no interior carve has fired yet), so the offset is unchanged and only the base register moves. */
            emit_text_n(_init, strlen(_init));
        } else {
            ef_b3(0x48, 0x8D, 0x05); bb_emit_patch_rel32(resume_init_lbl);
            { int z = emit_rec_rsp_arm() ? 4 : emit_rec_fb_num(), lo = z & 7; ef_b2((uint8_t)(0x48 | (z >= 8 ? 0x01 : 0x00)), 0x89); if (lo == 4) ef_b2(0x84, 0x24); else ef_b1((uint8_t)(0x80 | lo)); bb_emit_u32((uint32_t)(unsigned)g_suspend_resume_slot); }   /* REG-7 U5: fb twin — rsp (4) takes the SIB path, which is why the encoder already carries it */
        }
    }
    for (int _li = 0; _li < n; _li++) if (nodes[_li]->op == IR_LIMIT) {
        g_emit.op_off = drive_value_slot(nodes[_li]);
        bb_emit_x86(bb_limit_init());
    }
    for (int i = 0; i < n; i++) {
        if (g_emit.flat_stmt_frame && st_pre[i]) { g_emit.xa_bb_emit_pair_n = 2; g_emit.xa_bb_emit_pair_define[0] = st_x[i]; g_emit.xa_bb_emit_pair_jmp[0] = NULL; g_emit.xa_bb_emit_pair_define[1] = st_pre[i]; g_emit.xa_bb_emit_pair_jmp[1] = NULL; g_emit.op_fc_bytes = 0; bb_emit_x86((st_x[i] ? x86_deflabel_pair(0) + bb_glue_framed_leave() : std::string()) + x86_deflabel_pair(1) + bb_glue_framed_enter()); }   /* STMT-FRAME head stub: [st_x: cut the SOURCE bracket (mov rsp,___; pop ___) --] st_pre: ___ dance forward, fall through into the node's own α.  Cross-statement edges land st_x (leave-then-enter, the seed's stmt_*_end/_fail law); the prologue and nothing else falls into the first head's bare st_pre.  GLUE-4 (s21x-p): the bracket IS bb_glue_framed_* at K=0 -- op_fc_bytes zeroed HERE (driver-handoff, R5b) because this stub fires between statements where `_` still holds the previous node's K; the glue's pad arithmetic then reproduces the old x86_stmt_enter bytes exactly, and STATEMENT becomes the framed glue's first live customer (s21x-c law 4).  Both media via the pair 'E' records + the s21x-c encoders. */
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
        int gamma_is_beta = port_sz_beta(nodes[i]->γ.sz);
        int omega_is_beta = port_sz_beta(nodes[i]->ω.sz);
        int gamma_is_sig  = (nodes[i]->γ.sz[0] == (char)0xcf && (unsigned char)nodes[i]->γ.sz[1] == 0x83);   /* SN4-NARY-ALT "σ": inside-success → target's na_s glue */
        int gamma_is_phi  = (nodes[i]->γ.sz[0] == (char)0xcf && (unsigned char)nodes[i]->γ.sz[1] == 0x86);   /* SN4-NARY-ALT "φ": inside-fail → target's na_f glue */
        int omega_is_phi  = (nodes[i]->ω.sz[0] == (char)0xcf && (unsigned char)nodes[i]->ω.sz[1] == 0x86);
        { int _gg = 0; IR_t * _g = gtgt; while (_g && _g->op == IR_GOTO && _gg++ < 128) { if (!gamma_is_beta) gamma_is_beta = port_sz_beta(_g->γ.sz); if (!gamma_is_sig) gamma_is_sig = (_g->γ.sz[0] == (char)0xcf && (unsigned char)_g->γ.sz[1] == 0x83); if (!gamma_is_phi) gamma_is_phi = (_g->γ.sz[0] == (char)0xcf && (unsigned char)_g->γ.sz[1] == 0x86); gtgt = _g->γ.node; _g = gtgt; } }
        { int _gg = 0; IR_t * _o = otgt; while (_o && _o->op == IR_GOTO && _gg++ < 128) { if (!omega_is_beta) omega_is_beta = port_sz_beta(_o->γ.sz); if (!omega_is_phi) omega_is_phi = (_o->γ.sz[0] == (char)0xcf && (unsigned char)_o->γ.sz[1] == 0x86); otgt = _o->γ.node; _o = otgt; } }
        int omega_is_retry = omega_is_beta && !beta_is_stmt_land(otgt);   /* R1 β-TAG SPLIT: the RELEASE-SUPPRESSION consumers (UNWIND/ENDJMP/ZW5 steal guards below) read THIS, never the raw tag -- a β-edge into STATEMENT_BEGIN is a pure landing (normal release semantics, the fB exhaust class), a β-edge anywhere else is an in-match retry back-edge whose depth discipline the steals must not touch.  Label ROUTING (node_γ/node_ω = betas[k]) stays on the raw tag: both classes land on a β label. */
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
            if (gamma_is_sig) { node_γ = node_γ; }   /* SEQ-ERAD SE-5/SE-6: fc_seq_sigma_tgt deleted; LOWER wires σ-edges directly */
            if (gamma_is_phi) { node_γ = node_γ; }   /* SEQ-ERAD SE-5/SE-6: fc_seq_phi_tgt deleted; LOWER wires φ-edges directly */
            break;
        }
        { int _flk = emit_floater_kind(gtgt); if (_flk && node_γ == &lbl_γ) node_γ = emit_floater_label(_flk); }   /* FLOATERS: a jump into a floater from ANY chain resolves to the ONE bare program-wide label — the floater is excluded from this chain's nodes[], so the k-scan above cannot find it */
        if (nodes[i]->γ.node == NULL || nodes[i]->γ.node->op == IR_SUCCEED) node_γ = &lbl_γ;
        if (nodes[i]->γ.node && nodes[i]->γ.node->op == IR_FAIL) node_γ = &lbl_ω;
        int omega_resolved = 0;
        for (int k = 0; k < n; k++) if (nodes[k] == otgt) { node_ω = (omega_is_phi && na_f[k]) ? na_f[k] : omega_is_beta ? betas[k] : lbls[k]; omega_resolved = 1; break; }   /* SEQ-ERAD SE-5/SE-6: fc_seq_phi_tgt deleted */
        if (!omega_resolved) { int _flk = emit_floater_kind(otgt); if (_flk) { node_ω = emit_floater_label(_flk); omega_resolved = 1; } }   /* FLOATERS: ω twin of the γ fallback above */
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
            else for (int k = 0; k < n; k++) if (nodes[k] == rtgt) { g_move_label_tgt = wantb ? betas[k] : lbls[k]; if (wantb && g_emit_cfg && g_emit_cfg->pl_cells_graph) { g_emit.lbl_t1 = lbls[k]->name; g_emit.lbl_t1_p = lbls[k]; }   /* PL-ZK-4: stage α label as lbl_t1 for the cells arm -- bb_move_label's ZD arm pushes lbl_t1 (the callee α) to g_pl_retry instead of writing lbl_t0 (β) to FRQ(op_off+16).  On backtrack bb_indirect_goto's ZD arm calls rt_pl_retry_pop -> jmp rax to the α, which re-stages args and re-calls rt_proc_call_open_det (advances clause cursor internally).  ONE AUTHORITY: only this pre-pass site sets lbl_t1 for the PL retry path. */ break; }
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
              { flat_drive_match_alt(nodes, n, i, lbls, betas, na_s, na_f, fc_sig, node_γ, node_ω, &lbl_ω); }   /* SN4-NARY-SEQ rides the identical (entry,resume)×N + 2-glue pair layout; SEQ-ERAD SE-4: g_seq_static_cur/seqclean removed, static arm unconditional */
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
        if (nodes[i]->op == IR_GALT && nodes[i]->n_operands >= 2) {   /* RK-GRAM-3d: resolve arm labels before dispatch. operands[0]=arm-2(lbl_t1), operands[1]=arm-1(lbl_t0) */
            IR_t *arm2 = nodes[i]->operands[0]; IR_t *arm1 = nodes[i]->operands[1];
            g_emit.lbl_t0_p = NULL; g_emit.lbl_t0 = NULL;
            g_emit.lbl_t1_p = NULL; g_emit.lbl_t1 = NULL;
            if (arm1) for (int k = 0; k < n; k++) if (nodes[k] == arm1) { g_emit.lbl_t0_p = lbls[k]; g_emit.lbl_t0 = lbls[k] ? lbls[k]->name : NULL; break; }
            if (arm2) for (int k = 0; k < n; k++) if (nodes[k] == arm2) { g_emit.lbl_t1_p = lbls[k]; g_emit.lbl_t1 = lbls[k] ? lbls[k]->name : NULL; break; }
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
              if (_fk >= 0 && beta_is_stmt_land(nodes[_fk])) { _fk = -1; }   /* R1 β-TAG SPLIT (ZPOP-FOLD guard): a β-edge into STATEMENT_BEGIN is a pure landing pad -- the UNWIND LAW already freed own K rolling home, rsp is at claim base; there is nothing to fold and nothing to pop.  Entering the loop with _fk pointing at sbeg would exit with _sum=0 and g_emit.op_wpop=0, ERASING any UCLAIM wpop already staged (the 06e Layer 2 root cause: MATCH_BEGIN.ω now resolves to sbeg.β, the loop fires, flat_trivial_beta(STATEMENT_BEGIN)=false exits immediately, trailing assignment g_emit.op_wpop=0 stomps the staged release).  The fix is identity-preserving: no fold fires, op_wpop is untouched; the existing UCLAIM/ENDJMP/UNWIND release machinery does its job unchanged. */
              while (_fk >= 0 && _hops++ < 64 && flat_trivial_beta(nodes[_fk])) {
                  { long _fck = 0; if (fc_geom(nodes[_fk], &_fck)) _sum += _fck; }
                  IR_t *_c = nodes[_fk]; IR_t *_o = _c->ω.node;
                  int _ib = port_sz_beta(_c->ω.sz);
                  int _ip = (_c->ω.sz[0] == (char)0xcf && (unsigned char)_c->ω.sz[1] == 0x86);
                  { int _gg = 0; IR_t *_g = _o; while (_g && _g->op == IR_GOTO && _gg++ < 128) { if (!_ib) _ib = port_sz_beta(_g->γ.sz); if (!_ip) _ip = (_g->γ.sz[0] == (char)0xcf && (unsigned char)_g->γ.sz[1] == 0x86); _o = _g->γ.node; _g = _o; } }
                  bb_label_t *_next = NULL; int _k2 = -1;
                  for (int _k = 0; _k < n; _k++) if (nodes[_k] == _o) { _k2 = _k; break; }
                  if (_k2 >= 0) { _next = (_ip && na_f[_k2]) ? na_f[_k2] : _ib ? betas[_k2] : lbls[_k2]; }   /* SEQ-ERAD SE-5/SE-6: fc_seq_phi_tgt deleted; LOWER now wires φ-edges directly so no emit-time redirect needed */
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
          if (g_emit.flat_stmt_frame) {   /* STMT-FRAME retargets (SPD-2 idiom, one line above): edges into a statement head take its enter stub; edges out to the chain exits take the leave cuts.  Existing inline pops on those edges (the ΣK ω spends, the templates' own add rsp,N) become DEAD-BUT-HARMLESS: the cut's mov rsp,___ normalizes rsp regardless of arrival depth -- the seed's "fail edges jump the bracket cut, zero hand-counted pops" law delivered without touching a single template. */
              for (int _k = 0; _k < n; _k++) { bb_label_t * _st = st_x[_k] ? st_x[_k] : st_pre[_k]; if (_st && node_γ == lbls[_k]) node_γ = _st; if (_st && node_ω == lbls[_k]) node_ω = _st; }
              if (node_γ == &lbl_γ) node_γ = &lbl_stcγ; if (node_γ == &lbl_ω) node_γ = &lbl_stcω;
              if (node_ω == &lbl_ω) node_ω = &lbl_stcω; if (node_ω == &lbl_γ) node_ω = &lbl_stcγ; }
          if (nodes[i]->op == IR_MATCH_DEFER) {   /* SPD-2 op_scan criterion: sole frozen-pattern statement root = head's γ (GOTO-chased) is THIS node AND this node's ω resolved to the head's β */
              int _hk = -1; for (int _k = 0; _k < n; _k++) if (nodes[_k]->op == IR_MATCH_BEGIN) { _hk = _k; break; }
              if (_hk >= 0) { IR_t *_gt = nodes[_hk]->γ.node; { int _gg = 0; while (_gt && _gt->op == IR_GOTO && _gg++ < 128) _gt = _gt->γ.node; }
                  if (_gt == nodes[i] && node_ω == betas[_hk]) { g_emit.op_scan = 1; g_emit.op_scan_head_off = (int)drive_value_slot(nodes[_hk]); } }
          }
          int _uw_stolen = 0; long _uw_pop = 0;   /* ⭐⭐⭐ U-1 UNWIND (Lon HQ RULING 2026-08-03b, "failure is an UNWIND -- NEVER on failure do you whack"): a statement-exit fail edge stops flattening to a depth-known whack and instead rolls to the PREVIOUS fail-capable member's β with the LOCAL DIFFERENCE pop (zd_k+zd_wp)[i]-(zd_k+zd_wp)[p] -- own K plus the pure intermediate carves, the same fusion legality as the ΣK fold, never the global depth.  The DRIVE_PAIR β stub inherits the retarget+pop for free, which is the ruling's own point: a deterministic box's β IS its ω-continuation, and the two dead-β conventions were one hole.  The EARLIEST fail-capable member keeps its original edge untouched -- its (zd_k+zd_wp) pop to the original target IS the STATEMENT_BEGIN.β terminus fused over BEGIN's K=0 relay (BEGIN's ω is lowered NULL today; U-1b wires it and moves the terminus home).  Guards mirror ENDJMP's quartet; pred must be flat_trivial_beta (defined-β whitelist, the fold's own), same NODE-EXACT chased target (one statement, one fail exit -- a mid-extent :F(elsewhere) edge simply never finds a same-target pred and stays legacy, no depth arithmetic needed), in-statement (backward scan stops at the BEGIN/END bracket).  Killswitch SCRIP_UNWIND unset/0 = byte-identical legacy.  Supersedes ENDJMP + the ZW5 depth pool for the members it claims; U-3 deletes both once green at the merged head. */
          { static int _uw = -1; if (_uw < 0) { const char *_e = getenv("SCRIP_UNWIND"); _uw = (_e && *_e == '0') ? 0 : 1; }
            int _h3 = i;
            if (_uw && nodes[i]->op != IR_STATEMENT && nodes[i]->op != IR_STATEMENT_BEGIN && nodes[i]->op != IR_STATEMENT_END && zd_on[i] && zd_wp[i] > 0 && g_emit.op_wpop == 0 && !omega_is_retry && !omega_is_phi && !g_emit.flat_stmt_frame) {
                int _bg = 0; for (int _j = i - 1; _j >= 0; _j--) { int _o = (int)nodes[_j]->op; if (_o == IR_STATEMENT_BEGIN) { _bg = 1; break; } if (_o == IR_STATEMENT_END || _o == IR_STATEMENT) break; }
                IR_t *_ti = zd_chase(nodes[i]->ω.node);   /* cross-chain :F targets (labeled statements) are the COMMON case -- the target need not be in nodes[]; NODE-EXACT equality with the pred's chased target is the coherence guard (one statement, one fail exit), and the scan/STF/fold/β/φ retarget regimes are each excluded by their own conjunct above */
                if (_bg && _ti) for (int _j = i - 1; _j >= 0; _j--) {
                    int _o = (int)nodes[_j]->op; if (_o == IR_STATEMENT_BEGIN || _o == IR_STATEMENT_END || _o == IR_STATEMENT) break;
                    if (zd_on[_j] && zd_wp[_j] > 0 && flat_unwind_beta(nodes[_j]) && zd_chase(nodes[_j]->ω.node) == _ti) {
                        long _d = (zd_wp[i] + zd_k(nodes[i])) - (zd_wp[_j] + zd_k(nodes[_j]));
                        if (_d >= 0) { node_ω = betas[_j]; _uw_stolen = 1; _uw_pop = _d; } break; } } } }   /* ⭐ U-1b (2026-08-03c): flat_trivial_beta -> flat_unwind_beta.  The pred-selection walk is UNCHANGED; only the whitelist widens, and the mirror pre-pass above (grep "U-1b UNWIND β LIVENESS") guarantees every _j this line can select still owns an emitted β.  The two sites read the SAME predicate and the SAME conjuncts on purpose -- two spellings of one selection is the s22k skew disease, and here it would be a wild jump rather than a wrong number. */
          int _endj_stolen = 0;   /* END-JMP STEAL (Lon HQ directive 2026-08-03, func_call &TRIM witness -- three symptoms, one defect): a statement-exit ω at FULL EXTENT jumps to the statement's own IR_STATEMENT_END -- the SOLE release authority (WHACK CONTRACT item 4) -- instead of carrying the fused own-K flat_leave + wpop pair to the next head; the β stub (DRIVE_PAIR_DEF_JMP β,ω) inherits the retarget for free, erasing the two contradictory dead-β conventions (n26 pair-with-adds vs n63 bare-leak) in the same stroke.  GUARDS: armed non-UCLAIM member with planner statement-terminal wp (the ZW-5-steal precedent quartet), no pending trampoline ΣK fold (op_wpop==0 pre-staging -- END frees to statement entry, a chased Σ on top would double-free), depth equality zd_k+wp == END's gpop (excludes the n54-class mid-extent :F edges by arithmetic), NODE-EXACT successor equality through the IR_GOTO chase (excludes full-extent :F(elsewhere) edges), and node_ω still the chased target's plain α label (scan/STF regimes keep their own routing).  Own-K suppression = op_wsteal consumed at the ONE X86H_JMP/OMEGA flat_leave arm; wpop suppression = staged zero, the ZW-5 spelling.  Killswitch SCRIP_ZD_ENDJMP=0 = byte-identical revert. */
          { static int _ej = -1; if (_ej < 0) { const char *_e = getenv("SCRIP_ZD_ENDJMP"); _ej = (_e && *_e == '0') ? 0 : 1; }
            int _h2 = i;
            if (_ej && !_uw_stolen && nodes[i]->op != IR_STATEMENT && nodes[i]->op != IR_STATEMENT_BEGIN && nodes[i]->op != IR_STATEMENT_END && zd_on[i] && zd_wp[i] > 0 && g_emit.op_wpop == 0 && !omega_is_retry && !omega_is_phi && !g_emit.flat_stmt_frame) {
                int _e2 = -1; for (int _j = i + 1; _j < n; _j++) { if (nodes[_j]->op == IR_STATEMENT_BEGIN) break; if (nodes[_j]->op == IR_STATEMENT_END || nodes[_j]->op == IR_STATEMENT) { _e2 = _j; break; } }
                if (_e2 >= 0 && zd_gp[_e2] == zd_wp[i] + zd_k(nodes[i])) {
                    IR_t *_tf = zd_chase(nodes[i]->ω.node); IR_t *_te = zd_chase(nodes[_e2]->γ.node);
                    int _kf = -1; if (_tf) for (int _k = 0; _k < n; _k++) if (nodes[_k] == _tf) { _kf = _k; break; }
                    if (_tf && _tf == _te && _kf >= 0 && node_ω == lbls[_kf]) { node_ω = lbls[_e2]; _endj_stolen = 1; g_emit.op_wpop = 0; } } } }
          /* ZW-5 O-2: redirect node_ω to per-depth stub for uclaim-free armed nodes with non-zero fail depth */
          bb_label_t *_zw5_saved_omega = NULL; int _zw5_wpop_stolen = 0;
          { int _h = i;
          if (_zw5_on && !_endj_stolen && !_uw_stolen && nodes[i]->op != IR_STATEMENT && nodes[i]->op != IR_STATEMENT_END && zd_on[i] && zd_wp[i] > 0 && !omega_is_retry && !omega_is_phi) { for (int _si = 0; _si < zw5_pool_stmts; _si++) { int _si_base = 0; for (int _sj = 0; _sj < _si; _sj++) _si_base += zw5_stmt_cnt[_sj]; if (zw5_stmt_idx[_si] > i) { for (int _d = 0; _d < zw5_stmt_cnt[_si]; _d++) { if (zw5_stmt_depths[_si][_d] == zd_wp[i]) { _zw5_saved_omega = node_ω; node_ω = &zw5_pool[_si_base + _d]; _zw5_wpop_stolen = zd_wp[i]; break; } } break; } } } }
          if (zd_on[i] || zd_gp[i] > 0 || zd_wp[i] > 0) { g_zd_stage = 1; g_zd_arm = (zd_on[i] && !(g_emit_cfg && g_emit_cfg->pl_cells_graph && !g_emit.flat_all_zd)) ? 1 : 0; int _pl_suppress = (g_emit_cfg && g_emit_cfg->pl_cells_graph && !g_emit.flat_all_zd) ? 1 : 0; g_zd_gpop = _pl_suppress ? 0 : zd_gp[i]; g_zd_wpop = _pl_suppress ? 0 : (_uw_stolen ? (int)_uw_pop : ((_zw5_wpop_stolen || _endj_stolen) ? 0 : zd_wp[i])); g_zd_wsteal = _endj_stolen || _uw_stolen;   /* PL-ZK-3 ARM+RELEASE SUPPRESS preserved through M-3 merge: mixed armed+declined pl_cells_graph (flat_all_zd=0) has no carve, so arm and gpop/wpop must zero or ZOPQ/add-rsp corrupt. ONE AUTHORITY: same predicate at prologue carve suppressor. */   /* UCLAIM staged beside the ZD set -- one stage flag, one choke; mechanism-2 g_zd_uzwr staged alongside g_zd_uzw */
              g_zd_k = zd_on[i] ? zd_k(nodes[i]) : 0;   /* ZD-8 (b2) staged from zd_k -- THE ONE AUTHORITY, defined beside zd_nops.  This site and zd_plan's depth model were the SAME RULE SPELLED TWICE until s22k; both now call zd_k, so the planner's depth model and the emitted alpha carve cannot drift apart, and a change to the sink set is ONE edit rather than two-that-must-agree. */
              { g_zd_zunder = 0; if (zd_on[i] && nodes[i]->op == IR_MATCH_REPLACE) { int _zu = 0; for (int _zj = i - 1; _zj >= 0; _zj--) { if (nodes[_zj]->op == IR_MATCH_END) break; if (zd_on[_zj]) _zu += zd_k(nodes[_zj]); } g_zd_zunder = _zu; int _zp = 0, _inpat = 0; for (int _zj = i - 1; _zj >= 0; _zj--) { if (nodes[_zj]->op == IR_MATCH_BEGIN) break; if (nodes[_zj]->op == IR_MATCH_END) { _inpat = 1; continue; } if (_inpat && zd_on[_zj] && nodes[_zj]->op >= IR_MATCH && nodes[_zj]->op <= IR_MATCH_VALUE) _zp += zd_k(nodes[_zj]); } g_zd_zpat = _zp; { extern int fc_head_fp(const IR_t *); IR_t * _mb = (IR_t *)0; for (int _zj = i - 1; _zj >= 0; _zj--) { if (nodes[_zj]->op == IR_MATCH_BEGIN) { _mb = nodes[_zj]; break; } } int _fp = _mb ? fc_head_fp(_mb) : -1; g_zd_zfc = (_fp >= 0) ? _fp : 0; } { static int _zpd = -1; if (_zpd < 0) { const char * _e = getenv("SCRIP_ZPAT_DIAG"); _zpd = (_e && *_e == '1') ? 1 : 0; } if (_zpd) fprintf(stderr, "[ZPAT] i=%d zunder=%d zpat=%d zfc=%d zout_repl=%d op=%s\n", i, g_zd_zunder, g_zd_zpat, g_zd_zfc, zd_out[i], bb_op_name(nodes[i]->op)); } } }   /* L-3b STEP-6 FIX (this session): g_zd_zfc stages fc_head_fp(head) at the SAME choke g_zd_zpat uses -- the run-local MATCH_BEGIN is found by the identical backward-scan idiom the zunder/zpat blocks already use (bounded at MATCH_BEGIN), so this is the ONE place in the drive loop that can see both the head node AND fire before op_fc_disp resets for IR_MATCH_REPLACE's own case.  -1 (fc_head_fp's "no grant" sentinel) folds to 0, matching every other zd_z* field's byte-identical-when-absent convention.  See emit.h's op_zfc comment for the full root-cause chain (FINDING-2026-08-12h) and why this is a SEPARATE field from op_zpat, not a rename. */   /* ⭐ C-9 REPL-ZDEPTH (s35): the replacement expression runs AFTER MATCH_END and its cells are STILL LIVE at the splice (r9 = lea of rv's own cell, so they cannot be popped first).  bb_match_replace reads subject+span via FRQ/FR = [rsp+off+op_zdepth], but IR_MATCH_REPLACE is K=0 (zd_k -- it owns no result cell, correct) so op_zdepth was 0 and every one of those four reads was short by exactly this sum, landing on stale stack.  MEASURED against the planner's own zout: P1 (literal repl) zout(END)=16 → zout(REPL)=32, under=16; P8 (`A '-' B`) zout(END)=16 → zout(REPL)=96, under=80 -- and the gdb-measured offset deltas were exactly +16 and +80.  Backward scan to MATCH_END is the run-local spelling of zout(REPLACE)-zout(MATCH_END). ONE LINE ONLY per the s22k law. */ { g_zd_ztail = 0; if (zd_on[i] && (nodes[i]->op == IR_TO || nodes[i]->op == IR_TO_BY)) { int _zttail = 0; for (int _zt = i + 1; _zt < n; _zt++) { if (zd_on[_zt]) _zttail += zd_k(nodes[_zt]); } g_zd_ztail = _zttail; } }   /* ⭐ ZK-2 IR_TO BETA-RESUME DEPTH FIX (this rung): for IR_TO/IR_TO_BY on the cells arm, scan forward over ALL remaining armed nodes in nodes[] and sum their K values -- this gives the total depth pushed ABOVE IR_TO's K=32 block between IR_TO's α and any subsequent β resume.  All armed nodes in nodes[] for this graph are in the same non-popping run (the all-or-nothing gate ensures this), so the sum is exactly K_total - zd_out[IR_TO].  The scan is O(n) per IR_TO node; procs have at most one IR_TO per statement on the cells arm (the generator is the statement), so the cost is negligible.  g_zd_ztail is consumed at the emit_drive choke (line ~838) and cleared there after staging into op_ztail.  GATED zd_on[i]: declined nodes (op_zres=0) take the non-ZD arm in bb_to.cpp and never read op_ztail.  SN4 watermark: IR_TO is not emitted by lower_snobol4.c -- byte-identical by construction.  ONE AUTHORITY: this is the only site that computes g_zd_ztail. */
              for (int _zj = 0; _zj < 6; _zj++) { g_zd_read[_zj] = 0; g_zd_kind[_zj] = -1; }
              if (zd_on[i]) { int _no = zd_nops(nodes[i]);
                  int _op = (int)nodes[i]->op; int _cap = (_op == IR_MATCH_ASSIGN_COND || _op == IR_MATCH_ASSIGN_IMM);
                  for (int _zj = 0; _zj < _no && _zj < 6; _zj++) { if (_cap && _zj == 0) continue;   /* ZD-5b COND/IMM (s27 cross-front): operands[0] is the blob inner-pattern element (outside the ZD run, no depth entry); operands[1] is the SAVE box (value-spine run-predecessor carrying the cursor cell ZOPQ(1,8)); stage only slot 1, suppress slot 0 to avoid a spurious [rsp+0] resolution naming the box's own K=0 cell (which doesn't exist -- COND/IMM are K=0 sinks). */
                  IR_t * _p = nodes[i]->operands[_zj]; for (int _k = 0; _k < n; _k++) if (nodes[_k] == _p) { int _xh = 0; for (int _zm = i - 1; _zm > _k; _zm--) { if (nodes[_zm]->op != IR_MATCH_BEGIN) continue; extern int fc_head_fp(const IR_t *); extern int fc_tail_head(const IR_t *); if (x86_zc_frame() == ZC_FRAME_RSP && x86_port_mode() == ZC_PORT_FORTH && (fc_head_fp(nodes[_zm]) >= 0 || fc_tail_head(nodes[_zm]))) _xh += 32; } g_zd_read[_zj] = zd_out[i] - zd_out[_k] + _xh;   /* ⭐ L-4/061 CROSS-HEAD MATCH_BEGIN CARVE (this session, gdb-verified on crosscheck/capture/061_capture_in_arbno.sno at N=0 AND N=2, real linked mode-4 binary, not hand-derived): a producer before a run-local MATCH_BEGIN, read by a consumer after it, is short by exactly MATCH_BEGIN's own hfc() runtime carve -- sub rsp,32 at bb_match_begin.cpp:77 -- gated here by the IDENTICAL predicate bb_match_begin.cpp's hfc()/op_fc_wbytes uses (fc_head_fp>=0||fc_tail_head under ZC_FRAME_RSP+ZC_PORT_FORTH; mirrors emit.cpp:2720's g_zd_zfc computation for the same _mb).  That carve is INVISIBLE to zd_out because zd_k(IR_MATCH_BEGIN)=0 by design (no value cell of its own) -- ordinary armed nodes between producer and MATCH_BEGIN are NOT under-counted (K is additive; MATCH_BEGIN's own K=0 does not disturb the running sum), only the off-ledger carve is missing.  CORRECTS the prior ZD-5b-LEN comment at this site: its zd_ud/zdh fields exist nowhere in this tree (grep-confirmed, matches the s45 FINDING) and its worked number (deficit 48=32+16 for witness 061) does not reproduce -- gdb on the real linked binary measured the true deficit as 32 flat, and emitted+32 read back exactly N's coerced value on both witnesses.  Summed across every run-local MATCH_BEGIN found, not just the nearest, so a nested head is not silently under-corrected -- UNVERIFIED against an actual nesting witness, treat that arm as inherited-unverified until one exists.  Byte-identical when no cross-head MATCH_BEGIN exists (_xh=0): reduces to the prior zd_out[i]-zd_out[_k]. */ g_zd_kind[_zj] = (int)ir_norm_call_kind(_p->op); break; } } } }   /* ZD-1 staging (R5b driver-handoff): op_zread[k] = the DIFFERENCE OF TWO DEPTHS (s21x-u), zout[consumer] - zout[producer], both from the ONE walk.  ⛔ ZD-2d/2e FIX: this site RE-DERIVED the operand count inline (UNOP/ASSIGN?1:BINOP?2:0) instead of calling zd_nops, so the planner and the driver held TWO OPINIONS of how many operands a kind has -- the moment zd_nops grew COERCE_NUMERIC/CMP_TEST the planner ARMED their statements while this loop still computed _no=0, left g_zd_read all zero, and every ZOPQ/ZOPD rendered [rsp+0] = the box's OWN cell.  29 programs, every one silently wrong rather than crashing (rt_cmp_d comparing an uninitialized cell against itself).  ONE authority now; a kind added to zd_nops is staged here automatically.  K is no longer a second copy: s22k folded it into zd_k beside zd_nops, so BOTH one-authority laws now hold on this staging path. */
          if (nodes[i]->op == IR_MATCH_BEGIN) { g_emit.lbl_t0 = na_f[i] ? na_f[i]->name : NULL; g_emit.lbl_t0_p = na_f[i]; }   /* SEAL-UNWIND: hand BEGIN its own na_f so the drive can stage pair[3] for the template def */
          if (nodes[i]->op == IR_SUSPEND && g_emit.zframe_graph && g_emit_cfg && g_emit_cfg->icn_zframe_gen && g_suspend_dobody_beta) { g_zd_suspend_uclaim = 0; g_zd_stage = 1; }   /* M-3 MERGE: zd_uk/zd_uh (UCLAIM planner outputs) physically deleted; recarve value source gone -- staged 0 = the field's own byte-identical no-recarve path until ICN-FR re-plumbs from the cells-world depth model */   /* ICN-FR-5 SUSPEND-DO RECARVE: stage the statement head's uclaim via g_zd_suspend_uclaim+g_zd_stage so the choke applies it to g_emit.op_suspend_stmt_uclaim AFTER the per-node clear (g_emit.op_suspend_stmt_uclaim=0 at the choke runs before the staging apply, so direct pre-drive assignment is wiped).  g_suspend_dobody_beta is nonzero IFF the IR_SUSPEND has a do-body (set in the drive pre-pass at lines 2624-2628).  Byte-identical for non-zframe/non-dobody (g_zd_suspend_uclaim stays 0). */
          emit_drive(nodes[i], lbls[i], node_γ, node_ω, betas[i]);
          if (_zw5_saved_omega) node_ω = _zw5_saved_omega;
          if (_zw5_on && nodes[i]->op == IR_STATEMENT) { for (int _si = 0; _si < zw5_pool_stmts; _si++) { if (zw5_stmt_idx[_si] != i) continue; int _si_base = 0; for (int _sj = 0; _sj < _si; _sj++) _si_base += zw5_stmt_cnt[_sj]; for (int _d = 0; _d < zw5_stmt_cnt[_si]; _d++) { emit_label_define_bb(&zw5_pool[_si_base + _d]); bb_emit_x86(x86("add", "rsp", (long)zw5_stmt_depths[_si][_d]) + x86("jmp", "extlbl", (uint64_t)(uintptr_t)node_ω)); } break; } }   /* ZW-5 O-2: stub bodies; node_ω restored to fT */
          if (_dd && emit_text_count() == _p0) fprintf(stderr, "[DRIVE-DIAG] ZERO-EMIT chain=%d i=%d op=%s n_operands=%d\n", id, i, bb_op_name(nodes[i]->op), nodes[i]->n_operands); }
    }
    g_emit.op_beta_dead = 0;
    g_emit.op_wpop = 0;
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
        if (!bare) {
        emit_sep_rule('-'); emit_label_define_bb(&lbl_res);
        if (g_emit.flat_pat) {   /* W-MAP(3) res landing: the site jumped to [rsp+0]; the record is intact beneath.  Reload BOTH wires and drop 32B so the resumed interior speaks the same r10/r11 it was entered with.  r10/r11 are DESTINATIONS ONLY -- using either as scratch would destroy the wire it must return through. */
            if (g_is_text) { const char *_r = "mov r10, qword ptr [rsp + 8]\nmov r11, qword ptr [rsp + 16]\nadd rsp, 32\n"; emit_text_n(_r, strlen(_r)); }
            else { ef_b4(0x4C, 0x8B, 0x54, 0x24); ef_b1(0x08);
                   ef_b4(0x4C, 0x8B, 0x5C, 0x24); ef_b1(0x10);
                   ef_b4(0x48, 0x83, 0xC4, 0x20); }
        } else {

        if (g_is_text) {
            char _res[96];
            snprintf(_res, sizeof _res, "add rsp, 8\npop %s\n", emit_rec_fb());   /* REG-7 U5: the record[+8] payload is the FRAME BASE — ___ for RSP pat blobs (Lon FORTH ruling; zr is sealing to rsp and can no longer name it), r12/___ legacy modes byte-identical via the fb selector */
            emit_text_n(_res, strlen(_res));
        } else {
            int z = emit_rec_fb_num();
            ef_b4(0x48, 0x83, 0xC4, 0x08);
            if (z >= 8) ef_b2(0x41, (uint8_t)(0x58 | (z & 7))); else ef_b1((uint8_t)(0x58 | (z & 7)));
        }
        }
        }
    }
    if (!bare) {
    emit_sep_rule('-'); emit_label_define_bb(&lbl_β);
    {
        bb_label_t *resume_tgt = &lbl_ω;
        for (int i = 0; i < n; i++) if (nodes[i]->op == IR_SUSPEND) { resume_tgt = betas[i]; break; }
        if (g_emit_cfg && g_emit_cfg->body_root)
            for (int i = 0; i < n; i++) if (nodes[i] == g_emit_cfg->body_root && nodes[i]->op == IR_DISJUNCTION) { resume_tgt = lbls[i]; break; }
        if (g_emit_cfg && g_emit_cfg->body_root)
            for (int i = 0; i < n; i++) if (nodes[i] == g_emit_cfg->body_root && nodes[i]->op == IR_CALL_BUILTIN_GEN) { resume_tgt = betas[i]; break; }
        emit_jmp_label(resume_tgt, JMP_JMP);
    }
    }
    if (g_emit.flat_stmt_frame) { emit_sep_rule('-'); emit_label_define_bb(&lbl_stcγ); bb_emit_x86(bb_glue_framed_leave()); emit_jmp_label(&lbl_γ, JMP_JMP); emit_label_define_bb(&lbl_stcω); bb_emit_x86(bb_glue_framed_leave()); emit_jmp_label(&lbl_ω, JMP_JMP); }   /* STMT-FRAME chain-exit cuts: leave (mov rsp,___; pop ___) then jmp the port -- every retargeted γ/ω edge lands here; the cut normalizes rsp from ANY statement depth, which is what licenses leaving every template's inline pop untouched.  GLUE-4 (s21x-p): the cut is bb_glue_framed_leave -- takes no K by design, so it is byte-identical unconditionally. */
    { extern int g_flat_frame_floor; int _wire_stub = ((g_emit.flat_jmp_entry && g_flat_frame_floor > 0) || (g_emit.flat_lcl_proc && !(g_emit_cfg && g_emit_cfg->icn_cells_graph))) ? 1 : 0;   /* ZK-4 SLICE 1 (s215): CELLS-arm lexical procs are EXCLUDED from CLASS P and routed to CLASS ZF below. CLASS P needs a pcall record (rt_flat_ret_snap reads g_pcall_wires[g_pcall_top-1]); the cells-arm call site enters through the DC stub (rt_pl_dc_prep) which pushes none, so g_pcall_top==0 and every proc call died with core error 18 "Return from level zero". The cells prologue already writes the CLASS-ZF header triple, so the self-contained header read is both correct and cheaper. ZFRAME arm unaffected (icn_cells_graph=0 there by R-ZK-A); SNOBOL4 DEFINE stubs unaffected (they arrive via the floor>0 disjunct, not flat_lcl_proc). */  /* ICN-PROC-FRAME (s211): CLASS L (flat_lcl_proc) exits through wire header like CLASS P */   /* ICN-FR-2 CLASS ZF: zframe_graph graphs (Icon ζ-frame regime) exit through THEIR OWN wire header read — xa_flat_zframe_epilogue_γ/ω: lea rsp,[___+kt] unwind + load wire from [___+kt-24/-16] + restore caller ___ from [___+kt-8] + jmp.  This is the FR-3 direct-read shape provided in FR-2 so the simple quartet (f0/f1/fib) exits correctly.  CLASS ZF is distinct from CLASS P (_wire_stub → bb_glue_wire_γ/ω → rt_flat_ret_snap → pcall-array) and CLASS D (_blob_wire → resume-record push): ζ-frame graphs have no pcall record and no resume record at γ — they unwind directly.  Checked FIRST so CLASS ZF takes precedence over _wire_stub (both could be 0 for ζ-frame since floor=0 and flat_lcl_proc=0; the guard is belt-and-suspenders). */   /* ⭐ EXIT-CLASS LEDGER (s22v, the ___PAIR prerequisite "ledger the second authority" discharged): the shared γ/ω glue is selected by HOW THE GRAPH IS ENTERED, and the whack's licensees are now NAMED instead of accidental.  CLASS O (outer one-shot, α pinned ___ at the guarded framed_enter above): whack is the completion sync point OUTSIDE the graph -- keep.  CLASS C (chain-entered: LBL__ pseudo-procs + EVAL/CODE fragments, rt_chain_enter citizens): rt_chain_enter pins NO ___ -- the whack unwinds to the AMBIENT C -O0 frame pointer, which IS the m3 return-to-C mechanism 1016_eval depends on (the s22u ___PAIR falsification: suppressing it broke 1016_eval and fixed nothing) -- keep, now by ledgered decision.  CLASS P (wire-entered DEFINE stub blobs): rt_proc_call_open pushed a pcall record, the role-3 wire-adopt box (blob's first box) filled its wire quad, and the way home is THROUGH THE WIRES -- bb_glue_wire_γ/ω, no whack, no exit@PLT (the roman.s `call exit@PLT` inside a PROC that opened s22u).  THE DISCRIMINATOR IS THE DRIVER'S OWN STUB VERDICT: g_flat_frame_floor > 0 is set by all four scrip.c proc-emission loops from the role-3-entry predicate exactly and only around DEFINE-stub emissions (still live here; emit_jmp_entry_clear runs after) -- the same fact the CALL2BB _stfj discriminator uses, earned through the two falsifications its comment records (admitting ALL jmp-entry citizens regressed the EVAL/CODE family).  Chains and PAT$ blobs leave floor 0 and keep the whack. */
    int _blob_wire = (!_wire_stub && g_emit.flat_jmp_entry && g_emit.flat_pat) ? 1 : 0;   /* ⭐ CLASS D (s22z, the ledger's fourth citizen — DYNAMIC-FLAT, the s21x-f design's first customer IR_MATCH_PATREF(DELETED) delivered): PAT$ blobs are WIRE-ENTERED like CLASS P but with NO pcall record — bb_match_defer's fast arm and the dcap/defer replay loops all enter by bare `jmp rax` with rcx=γ-wire rdx=ω-wire, and the way home is through the wires the BLOB-GRANT α adopted into its own frame.  γ SUSPENDS the activation (frame + interior state retained for β re-entry): 16B resume record {res-landing, ___} pushed at the deep frontier — the invoker's β edge `jmp qword [rsp]` reads the landing word, the res stub restores ___ and falls into the chain's resume dispatch — then wires read through the PINNED ___ (depth-immune) and caller ___ restored self-referentially [kt-8] (U2b), reads-before-motion.  ω UNWINDS ABSOLUTELY: lea rsp,[___+kt] rejoins pre-entry rsp from ANY arrival depth (fence/ABORT seal cuts).  Spellings verbatim from the deleted epilogue's pat arm (git 1ba33ea6~1 xa_flat.cpp:172-174 TEXT / :185-199 BINARY); the CLASS O exit-glue these replace is the roman.s `call exit@PLT` disease the s22u/s22v ledger already named — a blob γ that exits the process.  floor>0 (DEFINE stubs) keeps CLASS P precedence: a DEFINE'd proc whose body carries a pattern is pcall-entered, not blob-entered. */
    if (!bare) {
    emit_sep_rule('-'); emit_label_define_bb(&lbl_γ);
    if (g_emit.zframe_graph || (g_emit_cfg && g_emit_cfg->icn_cells_graph && g_emit.flat_lcl_proc && g_emit.flat_jmp_entry)) { extern void xa_flat_zframe_epilogue_γ(void); xa_flat_zframe_epilogue_γ(); }   /* ZK-4 SLICE 1 (s215): CLASS ZF widened to the CELLS arm. Predicate mirrors xa_flat_class_zf() in xa_flat.cpp (the template-side ONE AUTHORITY); the header base differs per regime and is computed there by xa_flat_wire_hdr_base().  ZK-2 (s226): flat_jmp_entry conjunct added to cells-arm condition. The outer main graph has flat_jmp_entry=0 (entered from C via bare jmp, no wire installed in rcx/rdx); its γ exit is call exit@PLT, not wire-jmp. Procedures (noop, fib, etc.) have flat_jmp_entry=1 and exit via CLASS ZF wire-jmp. Without this guard, cells-arm main's γ would jmp through garbage rcx = SEGV. */   /* ICN-FR-2 CLASS ZF γ: direct wire-header read, depth-immune unwind (lea rsp,[___+kt]; load rcx=[___+kt-24]; restore ___=[___+kt-8]; jmp rcx).  BOTH MEDIUM via x86() in xa_flat.cpp.  Precedence over _blob_wire/_wire_stub: those are 0 for ζ-frame graphs by construction (floor=0, flat_lcl_proc=0, flat_pat=0). */
    else if (_blob_wire) {   /* W-MAP(3) (s22): PASS-THRU SUSPENSION RECORD.  The site's beta is `jmp qword ptr [rsp+0]` with a zero-guard, so a suspending blob MUST leave a resume landing at the frontier -- a bare `jmp r10` left the site jumping through whatever the granted pad held (the 26-SIGSEGV class).  Record is 32B ({res,r10,r11,pad}) so the spine stays 16-aligned; the wires are SAVED HERE and RESTORED IN res, per-activation ON THE SPINE, which is what makes a nested deferred re-entry survive where a flat global cell cannot (s14). */
        if (g_is_text) { const char *_g = "sub rsp, 8\npush r11\npush r10\n"; emit_text_n(_g, strlen(_g));
            { char _lr[96]; snprintf(_lr, sizeof _lr, "lea rax, [rip + %s]\npush rax\njmp r10\n", lbl_res.name); emit_text_n(_lr, strlen(_lr)); } }
        else { ef_b4(0x48, 0x83, 0xEC, 0x08);
               ef_b2(0x41, 0x53); ef_b2(0x41, 0x52);
               ef_b3(0x48, 0x8D, 0x05); bb_emit_patch_rel32(&lbl_res); ef_b1(0x50);
               ef_b3(0x41, 0xFF, 0xE2); }
    }
    else if (g_emit_cfg && g_emit_cfg->icn_cells_graph && g_emit.flat_lcl_proc && !g_emit.flat_jmp_entry) { int _bk = g_emit.flat_frame_bytes;   /* ZK-2 (s226): CELLS-ARM OUTER MAIN γ EPILOGUE. flat_lcl_proc prologue: sub rsp,kt; ...; mov ___,rsp — ___ = entry_rsp - kt. GLUE-O leave (mov rsp,___; pop ___) would restore rsp to entry_rsp-kt and pop from [___+0]=slot0=0, leaving rsp = entry_rsp-kt+8. Correct: lea rsp,[___+kt] = entry_rsp; pop ___ from [___+kt-8] = saved caller ___; then exit(0). */
        if (g_is_text) { char _seg[128]; snprintf(_seg, sizeof _seg, "and rsp, -16\nxor edi, edi\ncall exit@PLT\n"); emit_text_n(_seg, strlen(_seg)); }
        else { ef_b4(0x48, 0x83, 0xE4, 0xF0); ef_b3(0x31, 0xFF, 0x90); { uint64_t _ex = (uint64_t)(uintptr_t)(void *)exit; ef_b2(0x48, 0xB8); bb_emit_u64(_ex); ef_b2(0xFF, 0xD0); } } }
    else bb_emit_x86(_wire_stub ? bb_glue_wire_γ() : bb_glue_outer_γ());   /* ⭐⭐⭐ GAMMA-EXIT (this session): THE γ CHAIN HAD NO FINAL else AND ITS ω TWIN DID -- so every CLASS O graph emitted a BARE `main_γ:` label that fell straight through into main_ω's `mov edi,1; call exit@PLT`, and EVERY program exited 1 whether it succeeded or failed.  The success edge was correct, the landing was empty.  Diagnosed at 09e close (FINDING-2026-08-09e-fa-ab3a.md §7) as the board's second rc=1 source, distinct from the act_γ link break; measured here on the 22-program SNOBOL4 benchmark corpus, which read OK=0 CRASH=22 purely on rc with correct stdout on 18 of them.  The arm is the EXACT mirror of the ω dispatch tail below -- same _wire_stub discriminator, same two glue twins (bb_glue_wire_γ/bb_glue_outer_γ), both of which already existed and already emitted `xor edi,edi; call exit@PLT` for CLASS O and `mov eax,DT_S; ret` for CLASS C.  Nothing new is encoded here: the templates were right and unreachable.  SECOND-DISPATCHER law (AB-3a): a port's two landings are ONE authority and must be edited as a pair -- this is the third conviction of that law and the reason the chain is now symmetric arm-for-arm with ω. */
    }
    if (!bare) {
    emit_sep_rule('-'); emit_label_define_bb(&lbl_ω);
    if (g_emit.zframe_graph || (g_emit_cfg && g_emit_cfg->icn_cells_graph && g_emit.flat_lcl_proc && g_emit.flat_jmp_entry)) { extern void xa_flat_zframe_epilogue_ω(void); xa_flat_zframe_epilogue_ω(); }   /* ZK-4 SLICE 1 (s215): CLASS ZF ω widened to the CELLS arm; same predicate + same one-authority header base as the γ twin.  ZK-2 (s226): same flat_jmp_entry conjunct as γ — outer main exits via bb_glue_outer_ω (exit(1)), not wire-jmp. */   /* ICN-FR-2 CLASS ZF ω: same absolute-unwind shape; reads ω wire from [___+kt-16]. */
    else if (g_emit_cfg && g_emit_cfg->icn_cells_graph && g_emit.flat_lcl_proc && !g_emit.flat_jmp_entry) { int _bk = g_emit.flat_frame_bytes;   /* ZK-2 (s226): CELLS-ARM OUTER MAIN ω EPILOGUE. Symmetric with γ arm above; exit(1). */
        if (g_is_text) { char _seg[128]; snprintf(_seg, sizeof _seg, "and rsp, -16\nmov edi, 1\ncall exit@PLT\n"); emit_text_n(_seg, strlen(_seg)); }
        else { ef_b4(0x48, 0x83, 0xE4, 0xF0); ef_b3(0xBF, 0x01, 0x00); ef_b1(0x00); ef_b1(0x00); { uint64_t _ex = (uint64_t)(uintptr_t)(void *)exit; ef_b2(0x48, 0xB8); bb_emit_u64(_ex); ef_b2(0xFF, 0xD0); } } }
    else if (_blob_wire) bb_emit_x86(x86_jmp_reg("r11"));   /* ⭐⭐⭐ LADDER WREG-2 CLASS D ω: FAIL-AFTER-DRAIN = `jmp r11`.  The absolute unwind `lea rsp,[rax+kt]` dies because it existed to rejoin a pre-entry spine the α no longer left -- with no carve there is no depth to re-base from, and the FORTH fail-chain restores entry depth BY CONSTRUCTION (the drain law).  The ω wire read from the header dies for the same reason γ's did: the wire is in the register.  g_zctx POP dies with the push it balanced. */
    else bb_emit_x86(_wire_stub ? bb_glue_wire_ω() : bb_glue_outer_ω()); } }   /* OUTER-EXIT-1 (Lon s22p): the outermost box owns its OWN two ports, as glue -- this is the whole of what the deleted epilogue did in the ordinary case.  lbl_ω was left UNDEFINED by the s22o deletion while every fail edge still jumped to it, which is the single reason `--compile` produced no assemblable .s for 317/317 programs: one undefined symbol, not 317 defects.  ONE-SHOT BRIDGE (s22p): lbl_gamma and lbl_omega are jmp targets inside main -- no ret, no eax. bb_glue_outer_whack=true; framed_leave is the depth-immune exit; bb_glue_flat.cpp owns the protocol. */
    /* CARVE-KILL (Lon directive, s22o): XA_FLAT_EPILOGUE dispatch DELETED with its function.  The prologue died first and the epilogue was its surviving half -- it read [___+kt-24/-16/-8] and lea rsp,[___+kt] against a frame nobody established.  A graph has no prologue and no epilogue: the four ports are the whole protocol.  The six regimes this one function branched over (jmp-entry lexprep / nofill / eager, outer stmt-frame / plain, display-reg legacy) are SIX IR KINDS WITH SIX TEMPLATES, not six arms of one mega-function -- that is the rung this deletion opens. */
    /* PL-DC (REGAIN-1 SLICE C, s108): the per-proc DIRECT-CALL stub, appended after the shared exits when the driver armed the graph (g_flat_dc_np >= 0: registered det-lexical jmp-entry proc,
     * nparams<=4, hatch SCRIP_NO_DC unset — the SAME table facts the site predicate reads, so site and callee agree by construction).  Refinement guards here are LOUD: a driver-armed graph that
     * turns out not to be the lex jmp-entry shape is a predicate drift, not a codegen mode.  m3: the stub label's slab offset rides out in g_last_dc_off for the driver's rt_proc_set_dcfn(pfn+off)
     * registration at seal; m4: the named label is baked by the startup twin. */
    { extern int g_flat_dc_np; extern long g_last_dc_off; g_last_dc_off = -1;
      if (g_flat_dc_np >= 0) {
          if (!g_emit.flat_jmp_entry || !g_emit.flat_lex || g_emit.flat_gen || g_emit.flat_pat) { fprintf(stderr, "FATAL PL-DC: driver-armed graph is not det-lexical jmp-entry (jmp=%d lex=%d gen=%d pat=%d)\n", g_emit.flat_jmp_entry, g_emit.flat_lex, g_emit.flat_gen, g_emit.flat_pat); abort(); }
          bb_label_t lbl_dc; emit_label_initf(&lbl_dc, "%s_dcα", prefix);
          emit_sep_rule('-'); emit_label_define_bb(&lbl_dc);
          g_emit.flat_dc_body_p = (g_emit.zframe_graph || (g_emit_cfg && g_emit_cfg->icn_cells_graph && g_emit.flat_lcl_proc)) ? &lbl_α : &lbl_α_body;   /* ICN-FR-2: zframe dc stub jumps to proc_f_α (not body) so the ζ-frame prologue runs; non-zframe keeps the existing body jump.  ZK-4 SLICE 2: cells-arm CLASS ZF (icn_cells_graph && flat_lcl_proc) extended to also jump to lbl_α — flat_lcl_proc prologue allocates frame_total=flat_frame_bytes+(np+nl)*16 which must match xa_flat_wire_hdr_base()'s kt_adjusted used by the CLASS ZF epilogue.  Predicate mirrors xa_flat_class_zf() ONE AUTHORITY. */
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
        if (so < 0 || off < so) so = off; } }   /* SPD-NOFILL fix (s139) + CAP-NOFILL (s143): capture head cells (rt_cap_push/pop/top take &slot — empty state IS zero, read-before-first-push by design; the +0 buf ptr is deref'd after gen self-heal, so BOTH quads of the 16B grant must be zero) are implicit-zero citizens like resume/zeta-mark, NOT first-write-wins grants.  s139 pulled the zero suffix down to the lowest one, blanketing every grant above it (expression-eval PAT$: 560B rep stosb per activation = 52% of blob Ir); s143 records each site's offset in flat_cap_off for INDIVIDUAL zeroing by xa_flat's SPD-NOFILL arm, keeping flat_seed_off at the true resume/zeta-mark suffix.  SCRIP_CAP_NOFILL=0 or >48 distinct sites = the s139 pull-down.  s139 bisect provenance: 124 ([___+144]); explains 124/131/expr_eval/146.  Trio has no captures — no cost there. */
    g_emit.flat_layout_unknown = 0;
    if (rg <= 0) { rg = 4096; so = -1; g_emit.flat_layout_unknown = 1; }   /* SPD-NOFILL (s139): fallback graphs must keep the eager fill — see emit.h flat_layout_unknown */
    { extern int g_flat_frame_floor; if (g_flat_frame_floor > 0 && rg < g_flat_frame_floor) { rg = g_flat_frame_floor; so = -1; g_emit.flat_layout_unknown = 1; } }   /* SN4-FLAT-PROC (s176): a DEFINE stub's activation IS a fresh MAIN-layout frame — the body statements it transfers into address main's [___+off] slots, so the driver floors the stub's region at main's (zls_g_region on the shared graph, valid because drive_slots_all builds every graph's zls before any proc emits).  layout_unknown forces the eager full zero: main's implicit-zero suffix/capture citizens sit at MAIN offsets this stub's own tiny layout knows nothing about, and a fresh recursion frame must satisfy them all. */
    g_emit.flat_jmp_entry = 1; g_emit.flat_frame_bytes = (48 + (g_emit_cfg ? g_emit_cfg->jcon_value_region : 0) + 15) & ~15;   /* ⛔⭐⭐ CARVE-ERAD OVERREACH, CORRECTED (s22j).  This line read a HARD-CLAMPED `48` on the premise that "every BB now allocates its own storage at α through the ZW-1 hook, nothing is pre-allocated on behalf of anybody else".  THAT PREMISE IS FALSE FOR THE ZERO-CELL BOXES, and IR_MATCH_ALTERNATE is the witness: fc_geom DECLINES it, its own ALT-FLAT template header states "the box moves rsp nowhere", and its entire state is the FLAT zls quads FR(op_off)/FRQ(op_off+8)/FRQ(op_off+16).  A box that carves nothing keeps living in the graph slot region, so deleting that region did not free it -- it made the quads ALIAS the jmp-entry protocol's own save slots.  MEASURED on `PAT = ('aaa'|'bbb') ; S PAT`: kt=48 puts the protocol in the top 40 bytes (SCANBASE cursor kt-40, r8 kt-32, saved rcx/rdx/___ kt-24/-16/-8) leaving 8 bytes for the graph, while the ALT's quads sit at op_off 16/24/32 -- landing exactly on r8, rcx and rdx.  Observed ___=0x0 then an indirect jump through an ___ slot to address 0.  This is THE MODEL's own named failure mode ("DO NOT cut the sites before their readers are covered -- deleting the static authority under unconverted readers regresses programs"), and emit.h's flat_all_zd comment already states the rule this line broke: "jmp-entry blobs, flat_pat, flat_gen keep legacy verbatim (their protocols read the pinned whole-graph frame)".  jcon_value_region is still computed (emit.cpp:841/2673, xa_flat.cpp:131), so the region is restored rather than reinvented.  ⭐ 48 IS THE FLOOR, NOT THE FORMULA: the old spelling (32+rg+15)&~15 yields 32 at rg=0, which puts SCANBASE's kt-40 store at a NEGATIVE displacement; 48+rg keeps the wire header + 16B pad intact and is byte-identical to the clamp whenever rg==0.  The region shrinks on its own as kinds arm -- when the zero-cell families convert, rg goes to 0 and this returns to 48 without an edit here. */   /* ⭐ CARVE-ERAD (Lon directive: "remove the whole graph carve, we will not need any more").  WAS (32 + rg + 15) & ~15 -- 32B wire header PLUS rg = jcon_value_region, the WHOLE-GRAPH prefix-summed slot region.  rg IS the carve and it is DELETED: the residual 48 is the wire header (outside-γ at kt-24, outside-ω at kt-16, saved ___ at kt-8) plus 16B align pad, which is this box's OWN four-port protocol, not a graph frame -- law 1 permits a box a few bytes of its own.  Every BB now allocates its own storage at α through the ZW-1 hook and releases at γ/ω; nothing is pre-allocated on behalf of anybody else.  The old SCRIP_NO_GRAPH_CARVE=1 bisect flag simulated exactly this by clamping to 48 and is now the permanent and only behaviour, so it is deleted with the thing it clamped. */
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
extern "C" void emit_jmp_entry_clear(void) { extern int g_flat_frame_floor; g_flat_frame_floor = 0; g_emit.flat_jmp_entry = 0; g_emit.flat_frame_bytes = 0; g_emit.flat_seed_off = 0; g_emit.flat_layout_unknown = 0; g_emit.flat_pat = 0; g_emit.flat_lex = 0; g_emit.flat_gen = 0; g_emit.flat_deep_arrival = 0; g_emit.flat_cap_n = 0; g_flat_dc_np = -1; g_emit.flat_lcl_proc = 0; g_emit.zframe_graph = 0; }   /* ICN-FR-2: zframe_graph cleared with all other per-graph state (the per-graph flag must not bleed into the next graph through this choke). */
/* PROC-CONV (R12-FREE ladder rung 2) + NCB-1d (Lon "RSP/___ FORTH ζ for ALL, sharing the C stack", s90): ordinary procs arm the SAME jmp-entry regime.  DYN procs (SNOBOL4 DEFINE): args ride the name
 * dictionary, a self-allocated zeroed frame is correct.  DET LEXICAL procs (dyn_scope=0 — Icon/Raku): NCB-1d retires their call-regime trampoline — the blob self-allocates and its prologue calls
 * rt_jmp_frame_lexprep (NULVCL fill + rt_frame_bind_args from the staged g_call_args), so the caller-made-frame protocol is gone and flat_lex marks the graph for that prologue tail.  GENERATOR procs
 * are ADMITTED with flat_gen (GENP-SPINE s92): same lexprep prologue, but the γ epilogue RETAINS the activation on the spine (suspend record) — see emit.h flat_gen.  LBL__ main-program pseudo-procs are dyn_scope=0 but take no args and
 * are entered only through rt_chain_enter (rt_goto_transfer arm 4), which IS the jmp transfer — they arm with flat_lex=0 (no pcall record exists for lexprep to read). */
static int emit_graph_has_suspend(IR_graph_t *g) { if (!g) return 0; for (int i = 0; i < g->n; i++) if (g->all[i] && g->all[i]->op == IR_SUSPEND) return 1; return 0; }
static int emit_graph_has_deep_arrival(IR_graph_t *g) {   /* PIN-ALL RETIRED same session by FLATDISP-9 (Lon PIVOT: RSP zeta is the direction, ___ only for the intended frame constructs): the pin was the stopgap for the FR-vs-encoder double-add of op_flat_disp; with FR/FRQ/FRQB carrying RAW offsets the RSP regime is re-armed and re-measured.  Pin-all numbers preserved for the record: 313/2 · 311/2 · D0; re-enabling is one `return 1;` here.  MEASURED: crosscheck 304/11 · 303/10 · D3 with the census below vs 313/2 · 311/2 · D0 pinned -- the census-exempt RSP regime carried the FLATDISP class (rsp-relative FRQ with a baked op_flat_disp that over-counts cells popped before the reader; f3 witness: span read baked +128 against 32 live, so runtime-arg primitives read a virgin slot 72 bytes above their coerce), plus the 047/140/141 SEGV families and every DIVERGE.  ___-relative FRQ is depth-immune, so the whole class retires; FORTH cells still carve and pop identically -- only the fragile addressing goes dormant.  The census below is PRESERVED as the re-earn path: re-narrowing is deleting this line, licensed only by an fc-prefix audit that makes baked disp equal live depth at every FRQ reader.  Aligned with REG-7 U4 (unconditional-___ frame) and the ___-pin-main rung. */ if (!g) return 1; for (int i = 0; i < g->n; i++) { IR_t *c = g->all[i]; if (!c) continue; switch (c->op) { case IR_SUSPEND: case IR_SCAN: case IR_SCAN_ENTER: case IR_SCAN_ALTERNATE: case IR_SCAN_SEQUENCE: case IR_SCAN_UPTO: case IR_SCAN_FIND: case IR_SCAN_BAL: case IR_SCAN_MATCH: case IR_SCAN_MOVE: case IR_SCAN_TAB: case IR_TO: case IR_TO_BY: case IR_LIMIT: case IR_REPALT: case IR_PROC_GEN: case IR_CREATE: case IR_ITERATE: case IR_DISJUNCTION: case IR_CALL_BUILTIN_GEN: case IR_KEYWORD_ICON_GEN: case IR_MATCH_FENCE1: case IR_MATCH_ABORT: case IR_MATCH_ARBNO: case IR_MATCH_CALLOUT: case IR_MATCH_VALUE: return 1; case IR_MATCH_DEFER: return 1;   /* PATCTX (2026-07-29): the is_star gate DELETED -- census autopsy on 125_pat_json_literal: a NON-star sealed defer's γ-continuation arrives at blob-residue depth (PAT$γ pushes ___+res and jumps with rsp 16+blobframe LOW), yet its stamp restore is spelled FRQ(op_off), whose depth-immunity the s137 comment itself conditions on ___ ("FRQ(op_off), ___-relative → recursion-safe").  In the RSP regime that read only ever worked by SLOT-VALUE ALIASING: [rsp+mark] at residue depth landed on head.zls2_mark (+16), which holds the same rsp value -- the PATCTX grant widening moved the alias target onto head.sigma_save (outer r13 == 0 at the outermost match) and the program died at rsp=0, exposing the class.  Star was never the load-bearing property; γ-retention is, and every defer target can retain.  Same conservatism rule as the rest of the list: completeness IS the safety. */ default: break; } } return 0; }   /* FLATDISP-6 (s193, Lon's FOUR-CONSTRUCT ruling: STATEMENT/FUNCTION/ARBNO/FENCE1): the list is the ___-READER set established by template census — FENCE1 reads ___ as the dynamic-ζ floor (mov rsp,___ + watermark := ___); ARBNO BORROWS ___ as zv() the element-view register (bb_match_arbno 222/229/242 re-point it — Lon's "housekeeping about variable length children based off of ___", and WHY head/release/replace carry the +40 save/restore bracket); ABORT bypasses the β pop-cascade so its kill lands with pattern cells still carved (dynamic depth) — VISIBLE since ABORT-NODE (this session) relowered TT_ABORT from a bare IR_GOTO to the real IR_MATCH_ABORT; DEFER/VALUE σ-transfer into γ-retaining PAT$ blobs (post-transfer depth dynamic, the FORTH-ledger depth-immune watermark fact); CALLOUT conservative.  HEAD REMOVED (s193, the ABORT-NODE unlock): a fence/arbno/abort/defer-free match statement is depth-static — the match protocol rebalances rsp through the head snapshot at BOTH exits (058 .s ground truth), and head/release/replace's ___ instructions are save/restore-of-caller-value, value-neutral without a seed.  HEAD's earlier retention was measured, not ceremonial: dropping it BEFORE the abort node existed cost exactly 170/171 (m4 183→181→183, tripwire proven both directions).  Also dropped s193, subsumed then and free now: ALTERNATE, SEQUENCE, ARB, RETRY, RELEASE (comment-only in census).  FUNCTION is covered by the jmp-entry arm (seeds unconditionally, both media).  ⚠ COMMENT CORRECTION (prose-rot, RULES.md (a)-shape): the s192 text here claimed "every unlisted-but-present kind falls through to the SAFE answer (1)" — FALSE: default: break falls to return 0; the conservatism lives in the LIST being complete, which is why the census had to be by-template-read.  The crosscheck watermark is the tripwire, twice proven this session. */
static IR_t * g_fbm_nd[8192]; static unsigned char g_fbm_bit[8192]; static int g_fbm_n = 0;
static int emit_fb_bit_of(IR_t * nd) { for (int i = 0; i < g_fbm_n; i++) if (g_fbm_nd[i] == nd) return g_fbm_bit[i]; return 1; }   /* FB-STMT: unmapped node (foreign driver) stays ___ — conservative */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int emit_fb_stmt_scan(IR_graph_t * g) {   /* FB-STMT (Lon 2026-07-29): eligibility + per-node bit map.  Eligible iff every deep kind present is statement-bracketed (DEFER/ARBNO/FENCE1/VALUE); any suspend-class, generator-class, ABORT or CALLOUT kind disqualifies the whole graph (their arrivals are not rebalanced by the HEAD..RELEASE bracket).  Bits: every match-family op = 1 (pattern boxes and their quads are the housekeeping ___ set); every node inside a HEAD..RELEASE/REPLACE range CONTAINING a deep kind = 1 (pre-chains at match depth included by position); everything else = 0.  An unterminated HEAD disqualifies — degrade never die. */
    static int fbdbg = -1; if (fbdbg < 0) { const char * e = getenv("SCRIP_FB_DEBUG"); fbdbg = (e && *e == '1') ? 1 : 0; }
    g_fbm_n = 0; if (!g || g->n <= 0 || g->n > 8192) { if (fbdbg) fprintf(stderr, "[FB-STMT] bail size g=%p n=%d\n", (void *)g, g ? g->n : -1); return 0; }
    for (int i = 0; i < g->n; i++) { IR_t * c = g->all[i]; if (!c) continue; switch (c->op) {
        case IR_SUSPEND: case IR_SCAN: case IR_SCAN_ENTER: case IR_SCAN_ALTERNATE: case IR_SCAN_SEQUENCE: case IR_SCAN_UPTO: case IR_SCAN_FIND: case IR_SCAN_BAL: case IR_SCAN_MATCH: case IR_SCAN_MOVE: case IR_SCAN_TAB:
        case IR_TO: case IR_TO_BY: case IR_LIMIT: case IR_REPALT: case IR_PROC_GEN: case IR_CREATE: case IR_ITERATE: case IR_DISJUNCTION: case IR_CALL_BUILTIN_GEN: case IR_KEYWORD_ICON_GEN:
        case IR_MATCH_ABORT: case IR_MATCH_CALLOUT: { if (fbdbg) { extern const char * bb_op_name(IR_e); fprintf(stderr, "[FB-STMT] bail kind %s @%d n=%d\n", bb_op_name(c->op), i, g->n); } return 0; }
        default: break; } }
    for (int i = 0; i < g->n; i++) { IR_t * c = g->all[i]; g_fbm_nd[g_fbm_n] = c; g_fbm_bit[g_fbm_n] = (c && c->op >= IR_MATCH_LIT && c->op <= IR_MATCH_ADVANCE) ? 1 : 0; g_fbm_n++; }
    for (int h = 0; h < g->n; h++) { IR_t * hn = g->all[h]; if (!hn || hn->op != IR_MATCH_BEGIN) continue; int e = -1, deep = 0;
        for (int k = h + 1; k < g->n; k++) { IR_t * c = g->all[k]; if (!c) continue;
            if (c->op == IR_MATCH_DEFER || c->op == IR_MATCH_ARBNO || c->op == IR_MATCH_FENCE1 || c->op == IR_MATCH_VALUE) deep = 1;
            if (c->op == IR_MATCH_END || c->op == IR_MATCH_REPLACE) { e = k; break; } }
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
          int _callfam = (_c2c && (c->op == IR_CALL || c->op == IR_CALL_PROC_STAGED || c->op == IR_SAVE_RESTORE || c->op == IR_GOTO_DEFERRED)) ? 1 : 0;   /* + IR_GOTO_DEFERRED: a control-transfer SINK like IR_GOTO -- it jmps away through rt_goto_transfer/the registry and never returns to this statement, and every landing protocol it can reach (floaters' record restore, chain-enter) normalizes rsp/___ wholesale, so abandoned bracket depth is by-design (the same argument as the floaters' discarded-C-frames note in bb_save_restore.cpp). */   /* CALL2BB slice 3: the FUNCTION-construct admit (the s21x-e NEXT (b) rung) -- all three are self-allocating by construction (sr0 carves/releases its own save block per slice 2; the staged call box keeps rsp balanced on every arm and its exits ride the record protocol; floaters restore machine state wholesale from the record), so fc=0 on them is their FORTH form, not a flat-slot read.  Gated on SCRIP_CALL2BB so the STMT_FRAME-only world is byte-identical to s21x-e. */
          { extern int fc_vread_fp(const IR_t *); if (!_fc && c->op == IR_MATCH_BEGIN && fc_vread_fp(c) >= 0) _fc = 1;
            if (!_fc && c->op == IR_MATCH_END && c->n_operands > 0 && c->operands[0] && fc_vread_fp(c->operands[0]) >= 0) _fc = 1;   /* SUBJ-ARM-1: RELEASE licensed iff ITS statement's head is subject-cell licensed -- one authority; its frame reads are the head-field pair (window via op_fc_disp; PATCTX flat quartet written by head at the SAME post-unwind depth it reads them) */
            if (!_fc && c->op == IR_MATCH_LIT) _fc = 1; }   /* SUBJ-ARM-1: MATCH_LIT is ZERO frame refs by template inspection (register+RO compare only, no FR/FRQ in bb_match_lit.cpp) -- the zero-cell-by-law exempt set growing by measurement, the Icon ANY/POS precedent this comment block anticipated */   /* SUBJECT-CELL rung (a): HEAD is fc_geom 0 BY LAW (self-releasing 32B window, never summed) so its license under the regime is the fvr read registration -- the SAME registry the drive-case promotion reads, planner and classifier one authority */
          int _stfwire = (c->op == IR_GOTO || c->op == IR_STATEMENT_BEGIN || c->op == IR_STATEMENT_END) ? 1 : 0;   /* ⭐ MECHANISM-2 STF EXTENSION (SCRIP_ZW_RB): BEGIN/END are K=0 inert-wiring nodes (zd_k returns 0 for both; no frame reads; no value production) -- same exempt class as IR_GOTO (zeta_storage 923).  Without this, emit_stmt_frame_scan rejects them as UNREGISTERED, blocking STF for every pattern-bearing graph which has BEGIN/END bracketing every statement. */
          if (!_fc && !(c->op == IR_BINOP || c->op == IR_ASSIGN || _stfwire || _callfam)) { if (_sd) { extern const char * bb_op_name(IR_e); fprintf(stderr, "[STF] reject UNREGISTERED %s sn=%d\n", bb_op_name(c->op), sn); } return 0; } }   /* ACROSS-THE-BOARD conjunct (s21x-e, inverted polarity): under admit-all-kinds the producer LIST dies too -- ANY reachable fc=0 node rejects UNLESS it is a measured pure-TOS consumer (BINOP/ASSIGN, proven by the armed 28 incl. concat's str_concat_d C-call) or inert wiring (GOTO, zeta_storage 923).  The 097 autopsy generalized 026/027: IR_KEYWORD_SNOBOL4 / IR_CALL producers slipped a LIT/VAR-only conjunct and SEGV'd m4 identically (flat-slot arm presumes the graph carve).  END STATE: zeta_storage's fc registration is the ONE admission authority; widening the planner arms families here automatically, zero further classifier edits -- Lon's "not one at a time" delivered at the classifier.  Zero-cell-by-law kinds (the Icon ANY/POS precedent: fc_geom 0 IS their FORTH form) land conservative-reject until the exempt set grows by measurement. */   /* STMT-FRAME license conjunct 2 (026/027 root cause, this session): the whitelist admits the KIND; zeta_storage's fc registry independently picks the FLAVOR (the s191 FORTH registration -- fc_geom is the authority, final at this point because emit_chain runs zls_fct_finalize first).  A value-PRODUCER outside the registry takes the flat-slot template arm, whose [rsp + granted_off] addressing presumes the graph carve's region -- under the regime offset 40 at bracket depth is B+24 = THE CALLER'S RETURN ADDRESS, and the lit store shot it (026 m4 SEGV; m3 survived on entry-layout luck only).  Consumers (BINOP/ASSIGN) ride TOS and carry no own cell; requiring fc on them over-rejects (first fix attempt, measured on sl1).  The gate must read what the emitter will actually DO -- the FLATDISP-5b lesson again. */
        if (c->γ.node) q[qt++] = c->γ.node; }
    if (_sd) fprintf(stderr, "[STF] ADMIT reachable=%d (g->n=%d)\n", sn, g->n);
    return sn > 0 ? 1 : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int emit_graph_zstatic(IR_graph_t *g) { if (!g) return 0; for (int i = 0; i < g->n; i++) { IR_t *c = g->all[i]; if (c && (c->op == IR_MATCH_DEFER || c->op == IR_MATCH_VALUE)) return 0; } return 1; }   /* PS-1b (s151): exact emit-side mirror of runtime bb_graph_zstatic -- a DEFER/VALUE node is the only transfer into an arriving blob of unknown extent, so its absence is what makes the frame sound for ARBNO stride arithmetic; the region-known conjunct is applied at the capture site via flat_layout_unknown. */
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
    emit_chain_mark_entry_emitted(entry);   /* SN4-M34-5a: record BEFORE this call's own body walk so a later sibling's group-root pass (which pulls in OTHER labels' anchors -- this graph's own excluded by the `entry==g_emit_cfg->entry` guard) can skip an anchor this call already emitted standalone. */
    emit_chain_operand_refs(entry);
    if (g_emit_cfg) zls_fct_finalize(g_emit_cfg, 1);   /* PS-3 s153 LATE fct finalize: defer-bearing tail candidates were left PENDING by the early pass (drive_slots_all runs before any proc registers its patzeta ζ terms); HERE every earlier-emitted PAT$ is registered, so the SUSP license resolves -- price or decline-to-chain -- before this graph's first node emits (fc_tail_head is consulted at IR_MATCH_BEGIN). */
    g_bb_slotmap_n = 0;
    g_flat_chain_set_n = 0;
    { extern void bb_scc_handoff_reset(void); bb_scc_handoff_reset(); }   /* CALL2BB: per-graph handoff hygiene */
    { extern int g_scan_regs_live; g_scan_regs_live = 0; }   /* ICN-SCAN-SUSPEND-SYNC: scan regions are intragraph; without an emitted IR_SCAN leave box the flag leaked into later graphs, emitting sync brackets at non-scan call sites (r15.s main α garbage publish) */
    { extern void zop_audit_graph_close(void); zop_audit_graph_close(); g_emit.zop_seen = 0; }   /* ZOP-1 MIX AUDIT (Lon s21x-n): close the PREVIOUS graph's regime tally and re-arm.  This choke is where every other per-graph classification already lands, so the audit sees exactly the same graph boundary the pin/refine decisions do -- no separate notion of "graph" to drift out of sync. */
    g_emit.flat_all_zd = 0;   /* LP-1: cleared at the choke; codegen_flat_chain_body sets it from the pre-prologue kind scan */
    g_emit.flat_deep_arrival = emit_graph_has_deep_arrival(g_emit_cfg);   /* FLATDISP-5b (s193): classification moved HERE — FB-STMT rides the same choke point, next line */
    { int _rc_own = (g_emit_cfg && g_emit_cfg->resumable_callable) ? 1 : 0; g_emit.flat_fb_refine = (g_emit.flat_deep_arrival && !g_emit.flat_pat && !g_emit.flat_gen && !_rc_own) ? emit_fb_stmt_scan(g_emit_cfg) : 0;   /* FB-STMT guard reads THE GRAPH'S OWN resumable field, not g_resumable_callable_active — that global is refreshed only at jmp-entry arm (emit.cpp ~1821), so an outer main emitted after a PAT$ blob inherits the blob's 1 (measured on w1: hook skip rc=1 while mixed's RSUM proc happened to reset it).  The leak is benign for pinning (mains are deep anyway, emit_rec_pin unchanged) but starved the refinement; the stale-global class gets its own rung.  g_gen_proc_active dropped from the guard for the same reason — flat_gen (per-emission, cleared with flat_jmp_entry) is the honest generator signal at this choke point. */
      { static int fbdbg2 = -1; if (fbdbg2 < 0) { const char * e = getenv("SCRIP_FB_DEBUG"); fbdbg2 = (e && *e == '1') ? 1 : 0; } if (fbdbg2 && !g_emit.flat_fb_refine) fprintf(stderr, "[FB-STMT] hook skip deep=%d pat=%d gen=%d gp=%d rc=%d n=%d\n", g_emit.flat_deep_arrival, g_emit.flat_pat, g_emit.flat_gen, g_gen_proc_active, g_resumable_callable_active, g_emit_cfg ? g_emit_cfg->n : -1); } }   /* FB-STMT (Lon 2026-07-29): per-node data-ref refinement licensed only for statement-bracketed pins; suspend/heap-fb regimes keep whole-graph ___.  Unpinned graphs need no map (x86_fb_data short-circuits on the pin). */
    /* FLATDISP-5b continued (s193): classification moved HERE from emit_jmp_entry_for_proc — the WRONG-GRANULARITY of s192 was not the kind list but the ARMING COVERAGE: only jmp-entry procs were ever classified, while the graphs whose prologue actually READS the gate (outer call-regime mains at xa_flat's g_frame_active arm, PAT$ blobs via emit_jmp_entry_for_patproc, EVAL/CODE chains, rt pattern builds) inherited the cleared 0 and dropped the ___ seed their own fence/scan boxes then read (058: main seeds nothing, n11_match_fence1 does `mov rsp, ___` into the CRT frame).  emit_chain is the ONE choke point every graph emission passes, and every caller — driver loops, runtime_eval.c, bb_pat_build.cpp — sets g_emit_cfg to the graph immediately before, so this is the single writer; a null cfg falls through to the predicate's conservative 1.  Per-emission overwrite makes leaks from hand-armed callers (bb_pat_build restores fields manually, no emit_jmp_entry_clear) structurally impossible. */
    { static int _stf = -1; if (_stf < 0) { const char * e = getenv("SCRIP_STMT_FRAME"); _stf = (e && *e == '1') ? 1 : 0; }   /* ⛔⭐⭐ STF-UNFLIP (s22b, LON DIRECTIVE, and it is a MODEL correction not a tuning knob): DEFAULT FLIPPED BACK ON -> OFF; `SCRIP_STMT_FRAME=1` is now the OPT-IN and the regime ships DORMANT.  THE MODEL, restated in Lon's words because two sessions in a row encoded the opposite: NO BB EVER CONSUMES ANYTHING, EVER.  A BB allocates EXACTLY at alpha -- one `sub rsp, K`, its OWN cell -- never at beta (ARBNO-class excepted); it frees at omega; and the whole accumulation is freed AT THE END OF THE STATEMENT.  Cells stack up through the statement and ONE statement-terminal release drops all of them.  That release is op_zgpop, and it is the SOLE release authority.  ⛔ THE ERROR s21x-y AND s22a ENCODED: the per-statement ___ dance (bb_glue_framed_enter/leave) was made to stand in for that release, and s22a's ZGPOP-STF then ZEROED op_zgpop on armed graphs to stop the two colliding -- i.e. the bracket did not SUPPLEMENT the statement-terminal free, it REPLACED it, and a C-style frame pointer became per-statement ceremony on the value spine.  Law 4's "C-style ___ used occasionally" licenses ___ for CONSTRUCT housekeeping that must survive an unwind (ARBNO/FENCE1/CALL) -- it never licensed one bracket per statement.  MEASURED AT THIS HEAD, not inherited: crosscheck 318 m3 232/85 - m4 229/86/2 - DIV=1 {W04_arbno_basic}, IDENTICAL to the regime-on run, and the fail set is IDENTICAL BY SET in BOTH modes (diffed programmatically, not by count -- the test_string flake makes counts lie).  ⚠ NOT VACUOUS, and this is the instrument that proves it: an ___-INSTRUCTION census over all 318 crosscheck programs moves 15709 -> 15459 (250 instructions gone) and the count of programs containing ANY ___ moves 158 -> 127, i.e. EXACTLY 31 PROGRAMS GO ___-FREE -- the same 31 the SCRIP_STF_DEBUG=1 armed census names.  Witness `arithmetic.sno`: 26 ___ -> 0, statement `OUTPUT = A + B` now three alpha carves (`sub rsp,16` x3) and one terminal `add rsp,48`.  ⭐ THE REMAINING ___ DEBT IS A DIFFERENT CITIZEN AND IS THE NEXT RUNG: 15459 instructions across 127 programs is the flat_pat/flat_gen WHOLE-GRAPH PIN, not a statement bracket -- do not confuse the two (the s22a CENSUS-CORRECTED instrument law).  HISTORICAL, kept because the next session will otherwise re-derive it: s21x-y's flip was licensed by a watermark-neutrality argument that was TRUE and INSUFFICIENT -- neutrality proved the bracket broke nothing, never that it belonged. */   /* ⭐ STF-FLIP (s21x-y), SUPERSEDED BY THE ABOVE, retained for provenance: DEFAULT FLIPPED OFF -> ON, and `SCRIP_STMT_FRAME=0` is now the killswitch.  This is the moment the "C-style ___ used occasionally" of the design of record stops being dormant scaffolding and becomes the shipped regime: under it a STATEMENT is bracketed by the ___ dance (bb_glue_framed_enter/leave) and bb_match_begin's five POST-UNWIND-lifetime fields move from the whole-graph flat slots FRQ(op_off+40..72) onto HKQ = [___-48+8k], which is DEPTH-FREE BY CONSTRUCTION -- the value spine keeps riding RSP FORTH-style while exactly the housekeeping that must survive an unwind rides a stable base.  LICENSED BY THE CONDITION THIS LINE ITSELF NAMED ("default OFF until the watermark license ... fail-set diffed programmatically"), now satisfied and MEASURED AT THIS HEAD, not inherited: crosscheck 317 m3 232/85 - m4 229/86/2 - DIV=1 and broad 336 m3 244/92 - m4 238/90/8, and in BOTH corpora BOTH modes the fail set is BYTE-IDENTICAL to the regime-off run.  ⚠ NOT VACUOUS -- the trap that makes an identical watermark meaningless: the regime ARMS 31 GRAPHS IN 31 PROGRAMS (SCRIP_STF_DEBUG=1, live=1 census), so the neutrality is 31 armed customers agreeing, not a classifier declining everything.  s21x-r's 41-program regression is NOT being re-litigated here: it was ZLEAK-1, a process-scope `static int` from getenv applied to all 316 programs while emit armed 31, cured at SCRIP `efb72d69`; the armed set and that regression set were measured DISJOINT then, and this re-measurement at a later HEAD confirms the cure held. */
      extern int g_flat_outer_nparams; extern int g_flat_frame_floor; g_emit.flat_outer_nparams = g_flat_outer_nparams;
      g_emit.flat_lcl_proc = (!g_emit.flat_pat && !g_emit.flat_gen && g_emit_cfg && ((g_emit.flat_jmp_entry && (g_emit_cfg->nparams > 0 || g_emit_cfg->nlocals > 0)) || g_emit_cfg->icn_cells_graph)) ? 1 : 0;   /* ICN-PROC-FRAME (s211): jmp-entry graph with lexical locals or params requiring ___ seed + g_call_args install at α. Guards: NOT flat_pat (blob has its own BLOB-GRANT protocol); NOT flat_gen (generator keeps suspended activation, separate rung). Named for WHAT it decides. SNOBOL4: dyn_scope procs have no lexical locals (graph_has_local is always 0 there), so this fires only for Icon/Raku lex procs with params/locals.  ZK-2 FIX (s225): icn_cells_graph procs ALWAYS need flat_lcl_proc=1 — they exit CLASS ZF (wire-header read) which requires the flat_lcl_proc prologue to carve the wire-header frame (sub rsp,flat_frame_bytes + pin ___=rsp + install args).  Without this, a 0-param/0-local cells-arm proc (e.g. inc() in rung21) got flat_lcl_proc=0, bypassed the frame carve, and fell through to CLASS P / the Prolog DC stub (rt_pl_dc_prep) path — which pushes no pcall record and crashes.  ZK-2 FIX (s226): LIFT flat_jmp_entry REQUIREMENT FOR CELLS ARM. The s225 fix worked for jmp-entry procs (noop(), fib() — all have flat_jmp_entry=1). The outer main graph has flat_jmp_entry=0 and fell through to GLUE-O (push ___; sub rsp,8) — a 16-byte frame while FRQ slots extend to [___+184+], corrupting the C caller's frame and ___. icn_cells_graph alone is sufficient to require flat_lcl_proc: the cells arm's CLASS ZF epilogue reads wire header at [___+kt-24/-16/-8], which requires the flat_lcl_proc prologue's sub+save+pin regardless of whether the graph was entered via jmp-entry or as the outer main. The flat_jmp_entry conjunct is retained ONLY for the SN4/Raku nparams/nlocals arm (non-cells jmp-entry procs still require it). TWO CONDITIONS: (1) jmp-entry with params/locals as before; (2) icn_cells_graph regardless of jmp-entry. SN4/Prolog/Raku/Pascal: icn_cells_graph=0 — byte-identical. CELLS=0: icn_cells_graph=0 — byte-identical. ONE AUTHORITY. */
      g_emit.zframe_graph = (g_emit_cfg && g_emit_cfg->zframe_graph && !g_emit_cfg->icn_cells_graph) ? 1 : 0;   /* ICN-FR-2: copied from IR_graph_t at the ONE choke point. ZK-3 ADDITIVE: icn_cells_graph suppresses zframe_graph for cells-arm graphs — R-ZK-A mutual exclusion law (lower_icon.c sets icn_cells_graph XOR zframe_graph, never both); cells-arm ZOPQ reads use FORTH-spine RSP-relative offsets which are correct regardless of the zframe pre-allocation, so the zframe prologue (sub rsp,flat_frame_bytes; mov ___,rsp) must NOT fire for cells-arm graphs — it displaces RSP before the ZD depth model starts and ZOPQ reads land at wrong offsets (write(1 to 3) prints 0 not 1 without this guard). ONE CHOKE POINT (the s193 discipline: one writer, per-emission, structurally leak-proof). 0 for all SN4/Prolog/Raku/Pascal graphs by construction (IR_alloc calloc-zeroed, other lowerers never set it). Guard: if g_emit_cfg is null (e.g. rt pattern builds that arm manually) defaults to 0 — the safe direction; those callers are Icon-lowerer-independent. */
      if ((g_emit.zframe_graph || (g_emit_cfg && g_emit_cfg->icn_cells_graph)) && g_emit.flat_frame_bytes == 0) { g_emit.flat_frame_bytes = (48 + (g_emit_cfg ? g_emit_cfg->jcon_value_region : 0) + 15) & ~15; }   /* ICN-FR-2 MAIN-GRAPH FIXUP: the Icon main graph is emitted via g_emit_cfg=bbg; emit_chain(...) without any emit_jmp_entry_for_proc call (both m3 and m4 driver paths), so flat_frame_bytes is 0 at the choke.  Proc graphs DO go through emit_jmp_entry_for_proc -> emit_jmp_entry_arm_region which sets flat_frame_bytes before emit_chain; for them this guard fires as a no-op (flat_frame_bytes already >0).  Formula mirrors emit_jmp_entry_arm_region's formula: 48 floor + jcon_value_region rounded up to 16.  ZK-2 FIX (s226): extended from zframe_graph-only to also cover icn_cells_graph — cells-arm main has zframe_graph=0 (R-ZK-A) and flat_frame_bytes=0 before this point; without the fixup flat_lcl_proc's frame_total=0 → sub rsp,0 → FRQ slots at [___+96+] land in caller's C frame. ONE AUTHORITY. */
      int _c2g; { static int _c2 = -1; if (_c2 < 0) { const char * e = getenv("SCRIP_CALL2BB"); _c2 = (e && *e == '1') ? 1 : 0; } _c2g = _c2; }
      int _stfj = (_stf && _c2g && g_emit.flat_jmp_entry && !g_emit.flat_pat && !g_emit.flat_gen && !g_emit.flat_deep_arrival && !g_gen_proc_active && !(g_emit_cfg && g_emit_cfg->resumable_callable) && g_flat_frame_floor > 0) ? 1 : 0;   /* DEFINE-STUB DISCRIMINATOR (measured this session, two falsifications deep: admitting ALL jmp-entry citizens regressed expr_eval/161/1016/1019 -- runtime EVAL/CODE chains speak their own enter/exit protocol, not the pcall record's; keying on the emit_chain entry node's role-3 shape then missed the m4 driver path, whose entry parameter is not the wire-adopt node).  g_flat_frame_floor > 0 is the driver's OWN stub verdict: all four proc-emission loops (scrip.c 882/1340/1528/1633) set it from the role-3-entry predicate exactly and only around DEFINE-stub emissions, it is still live at this choke (emit_jmp_entry_clear runs after), and every other jmp-entry citizen -- PAT$ patprocs, EVAL/CODE chains, rt pattern builds -- leaves it 0.  Conservative edge: an empty-main program floors 0 and declines, which is the safe direction. */   /* CALL2BB slice 3 (Lon s21x-c laws 5/6/7: DEFINE constant-folded = IR_SAVE_RESTORE + IR_CALL; the whole-graph carve is the NAMED ANTI-PATTERN; statement scope is the planning ceiling): a jmp-entry DEFINE/LBL__ blob whose reachable body passes the SAME classifier rides the statement regime -- body statements self-allocate per-BB, so the blob needs NO region, NO fill, NO main-layout floor.  deep/pat/gen keep legacy verbatim (their record/suspend protocols read the pinned whole-graph frame). */
      int _pat_ok = (!g_emit.flat_pat) ? 1 : 0;   /* ⭐ MECHANISM-2 STF EXTENSION (SCRIP_ZW_RB): when mechanism-2 is enabled, flat_pat=1 graphs are admitted.  STF fires push ___ at statement head (pre-all-cells); mechanism-2's push ___ in MATCH_BEGIN nests inside it (nested indeterminacy = nested frames, HQ ruling 2026-08-03).  Default OFF (zw_rb_on()=0) preserves !flat_pat exclusion byte-identical. */
      g_emit.flat_stmt_frame = ((_stf && !g_emit.flat_jmp_entry && _pat_ok && !g_emit.flat_gen && !g_gen_proc_active && !(g_emit_cfg && g_emit_cfg->resumable_callable) && g_emit.flat_outer_nparams == 0) || _stfj) ? emit_stmt_frame_scan(g_emit_cfg, entry) : 0;
      { static struct { const void * g; const void * e; int v; } _sv[1024]; static int _sn = 0; static int _strict = -1; if (_strict < 0) { const char * e = getenv("SCRIP_STF_STRICT"); _strict = (e && *e == '1') ? 1 : 0; } if (_stf && g_emit_cfg) { int _f = -1; for (int i = 0; i < _sn; i++) if (_sv[i].g == (const void *)g_emit_cfg) { _f = i; break; }
        if (_f < 0) { if (_sn < 1024) { _sv[_sn].g = (const void *)g_emit_cfg; _sv[_sn].e = (const void *)entry; _sv[_sn].v = g_emit.flat_stmt_frame; _sn++; } }
        else { if (_sv[_f].v != g_emit.flat_stmt_frame) { fprintf(stderr, "WARN STF-STABILITY: graph %p verdict %d then %d -- the per-graph verdict is NOT stable across emissions; ZLEAK-3's plan-time fixpoint would mis-decide here (witness: 1014_func_freturn under CALL2BB)\n", (const void *)g_emit_cfg, _sv[_f].v, g_emit.flat_stmt_frame); if (_strict) abort(); }
              if (g_emit.flat_stmt_frame && _sv[_f].e != (const void *)entry) { fprintf(stderr, "WARN STF-SINGLE-ENTRY: ARMED graph %p emitted from entries %p and %p -- arming-implies-single-entry violated\n", (const void *)g_emit_cfg, _sv[_f].e, (const void *)entry); if (_strict) abort(); } } } }   /* ZLEAK-3 INVARIANT GUARD (s21x-r): the two premises the plan-time register/scan/unregister fixpoint rests on, MEASURED over the 317-program corpus and now ENFORCED rather than assumed -- (1) VERDICT STABILITY: a graph gets the same verdict at every emission (0 violations / 32 multi-choke graphs); (2) ARMING IMPLIES SINGLE ENTRY: every armed graph is emitted from exactly one entry (44 decisions / 44 distinct armed graphs, 1:1), while multi-entry graphs -- up to 11 entries, all DEFINE/indirect-goto/EVAL shapes -- decline at every entry.  Together these are what let ONE plan-time decision match EVERY emission: the planner scans from g->entry, and for any graph it would arm that entry is the only one, while a graph it declines is declined at all of them.  The strong premise the s21x-r cursor first wrote (plan-time entry == emit-time entry) is FALSE and is NOT what the fixpoint needs.  Regime-gated on _stf so the default path never executes this.  If either abort ever fires, ZLEAK-3 must be re-derived before it is trusted -- that is the whole point of spending four lines here. */
      if (g_emit.flat_stmt_frame && _stfj) { g_emit.flat_frame_bytes = 48; g_emit.flat_seed_off = 0; g_emit.flat_layout_unknown = 0; g_emit.flat_cap_n = 0; }   /* CALL2BB slice 3 kt override: 32B wire header + slot0 only (xa_flat's >=48 invariant is the floor) -- retires the s176 main-region floor (roman's 1344).  Every wire park/read and both epilogue arms are kt-parametric (flat_frame_bytes is the single source), so 48 is consistent at the prologue, the role-3 wire-adopt, and the determinate exits; the RETURN/FRETURN floaters restore the CALLER-side record rsp/___ wholesale, abandoning this 48B header by design. */
      { static int _sd2 = -1; if (_sd2 < 0) { const char * e = getenv("SCRIP_STF_DEBUG"); _sd2 = (e && *e == '1') ? 1 : 0; } if (_sd2 && _stf) fprintf(stderr, "[STF] choke stf=%d jmp=%d pat=%d gen=%d gp=%d rc=%d np=%d deep=%d floor=%d c2=%d stfj=%d -> live=%d g=%p entry=%p\n", _stf, g_emit.flat_jmp_entry, g_emit.flat_pat, g_emit.flat_gen, g_gen_proc_active, (g_emit_cfg && g_emit_cfg->resumable_callable) ? 1 : 0, g_emit.flat_outer_nparams, g_emit.flat_deep_arrival, g_flat_frame_floor, _c2g, _stfj, g_emit.flat_stmt_frame, (void*)g_emit_cfg, (void*)entry); } }   /* STMT-FRAME (s21x-c): classified at the ONE choke point every graph emission passes (the FLATDISP-5b single-writer discipline, same line as FB-STMT above); env-gated SCRIP_STMT_FRAME=1, default OFF until the watermark license (may only add passes, fail-set diffed programmatically).  Guard conjuncts mirror the FB-STMT guard: outer call-regime mains only for slice 1 -- jmp-entry citizens, blobs, generators, resumable and param-bearing graphs keep legacy verbatim (the classifier whitelist already excludes their kinds; the conjuncts make the slice boundary explicit and greppable). */
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
    /* RTX-FUNC-0 POST-HOOK: caller can set g_emit_chain_posthook to append additional JIT content into this
     * session (same bb_emit_buf, live label pool) before emitter_end() seals and patches.  Used by the m3
     * main-graph path to emit activation blocks so fn_cells resolve before program execution.  One-shot:
     * cleared here so proc-chain emit_chain calls (which run before main) never fire the hook. */
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
void gva_collect_graph(IR_graph_t *g) { if (!g) return; for (int i = 0; i < g->n; i++) { IR_t *nd = g->all[i]; if (!nd) continue; if ((nd->op == IR_VAR || nd->op == IR_ASSIGN || nd->op == IR_VAR_REF) && IR_LIT(nd).sval) { const char *_gn = IR_LIT(nd).sval; if (!strncmp(_gn, "PATV$", 5)) continue;   /* PB-1s: stage-2 snapshot globals are NV-only — GVA would split the write (ASSIGN→GVA) from the read (DEFER fallback→NV), causing the DEFER to see the uninitialised NV slot */ (void)gva_collect_var(_gn); } } }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void gva_collect_icon_globals(void) {
    extern const char *global_names[]; extern int global_count;
    for (int i = 0; i < global_count; i++) if (global_names[i] && strncmp(global_names[i], "PATV$", 5) != 0) (void)gva_collect_var(global_names[i]);   /* PB-1s: PATV$ snapshot globals excluded (NV-only, same reason as gva_collect_graph exclusion) */
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
